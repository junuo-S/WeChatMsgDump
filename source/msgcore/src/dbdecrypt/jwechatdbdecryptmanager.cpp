#include "jwechatdbdecryptmanager.h"

#include <QDir>
#include <QDirIterator>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QThreadPool>

#include <junuobase/utils/junuobaseutils.h>

#include "dbdecrypt/decryptor/jwechatdbdecryptorfactory.h"
#include "dbdecrypt/decryptor/jabstractwechatdbdecryptor.h"
#include "dbdecrypt/processreader/jwechatprocessreaderfactory.h"
#include "dbdecrypt/processreader/jabstractwechatprocessreader.h"
#include "dbdecrypt/combiner/jwechatdbcombiner.h"

static QStringList GetDBFileList(const WeChatMajorVersion majorVersion, const QString& baseDir)
{
	QStringList fileList;
	static const QMap<WeChatMajorVersion, QString> s_databaseListMap = {
		{WeChatMajorVersion::Version_3, ":/configs/databasesv3.json"},
		{WeChatMajorVersion::Version_4, ":/configs/databasesv4.json"}
	};
	QFile jsonFile(s_databaseListMap.value(majorVersion));
	if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return fileList;
	QByteArray jsonData = jsonFile.readAll();
	jsonFile.close();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
	jsonData.clear();
	if (jsonDoc.isNull() || !jsonDoc.isArray())
		return fileList;
	for (const auto& dbFileItem : jsonDoc.array().toVariantList())
	{
		auto dbFileInfo = dbFileItem.toMap();
		if (!dbFileInfo.value("needDecrypt").toBool())
			continue;
		QString relativePath = dbFileInfo.value("relativePath").toString();
		QString fileName = dbFileInfo.value("fileName").toString();
		bool fuzzy = dbFileInfo.value("fuzzy").toBool();
		QDir dir(baseDir);
		dir.cd(relativePath);
		if (fuzzy)
		{
			QRegularExpression regex(fileName);
			QDirIterator dirIter(dir.absolutePath(), QDir::NoDotAndDotDot | QDir::Files | QDir::NoSymLinks);
			while (dirIter.hasNext())
			{
				QFileInfo info(dirIter.next());
				if (regex.match(info.fileName()).hasMatch())
					fileList.append(info.absoluteFilePath());
			}
		}
		else
		{
			if (dir.exists(fileName))
				fileList.append(dir.absoluteFilePath(fileName));
		}
	}
	return fileList;
}

static bool CreateDirIfNeed(const QString& path)
{
	QDir dir(path);
	if (dir.exists())
		return true;
	return dir.mkpath(path);
}

STDMETHODIMP_(bool) JWeChatDBDecryptManager::StartReadWeChatProcess()
{
	QThreadPool::globalInstance()->start([this]()
		{
			JCommonAsyncEvent event;
			event.m_subType = JCommonAsyncEvent::SubType::SubType_End;
			event.m_type = EventType::Event_ReadProcess;
			event.m_extraData.insert("processId", m_processReader ? m_processReader->GetWeChatProcessId() : 0);
			event.m_extraData.insert("executablePath", m_processReader ? m_processReader->GetWeChatExecutablePath() : QString());
			event.m_extraData.insert("version", m_processReader ? m_processReader->GetWeChatVersion() : QString());
			event.m_extraData.insert("nickName", m_processReader ? m_processReader->GetNickName() : QString());
			event.m_extraData.insert("userName", m_processReader ? m_processReader->GetWeChatUserName() : QString());
			event.m_extraData.insert("phoneNumber", m_processReader ? m_processReader->GetPhoneNumber() : QString());
			event.m_extraData.insert("wxid", m_processReader ? m_processReader->GetWxid() : QString());
			event.m_extraData.insert("dataPath", m_processReader ? m_processReader->GetDataPath() : QString());
			this->Notify(&event);
		});
	return true;
}

STDMETHODIMP_(bool) JWeChatDBDecryptManager::StartDecryptDataBase()
{
	if (!m_processReader || m_processReader->GetDataPath().isEmpty())
		return false;
	QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
	dir.cd(m_processReader->GetWxid());
	if (!CreateDirIfNeed(dir.absolutePath()))
		return false;
	m_finalDBFileName = dir.absoluteFilePath("merged_db.db");
	if (QFile::exists(m_finalDBFileName) && !QFile::remove(m_finalDBFileName))
		return false;
	QThread* th = QThread::create([this, dir]()
		{
			const QStringList& dbFileList = GetDBFileList(m_majorVersion, m_processReader->GetDataPath());
			const float totalCount = dbFileList.size();
			QList<QPair<QString, QString>> ret;
			QDir innerDir = dir;
			innerDir.mkdir("decrypted");
			innerDir.cd("decrypted");
			std::atomic<int> decryptedCount = 0;
			for (const QString& fileName : dbFileList)
			{
				QString outputFileName = innerDir.absoluteFilePath(QFileInfo(fileName).completeBaseName());
				ret.append(QPair(fileName, outputFileName));
				QThreadPool::globalInstance()->start([totalCount, this, fileName, outputFileName, &decryptedCount]()
					{
						std::unique_ptr<JAbstractWeChatDBDecryptor> decryptor(JWeChatDBDecryptorFactory::createInstance(m_majorVersion));
						decryptor->SetParam(fileName, outputFileName, m_processReader->GetKey());
						decryptor->Decrypt();
						JCommonAsyncEvent event;
						event.m_type = EventType::Event_Decrypt;
						event.m_subType = JCommonAsyncEvent::SubType::SubType_Progress;
						event.m_progress = ++decryptedCount / totalCount;
						this->Notify(&event);
					});
			}
			QThreadPool::globalInstance()->waitForDone();

			std::atomic<int> combinedCount = 0;
			std::unique_ptr<JWeChatDBCombiner> combiner = std::make_unique<JWeChatDBCombiner>();
			combiner->beginCombine(m_finalDBFileName);
			for (const auto& each : ret)
			{
				combiner->combineFile(each.second);
				JCommonAsyncEvent event;
				event.m_type = EventType::Event_Combine;
				event.m_subType = JCommonAsyncEvent::SubType::SubType_Progress;
				event.m_progress = ++combinedCount / totalCount;
				this->Notify(&event);
			}
			combiner->endCombine();
			JCommonAsyncEvent event;
			event.m_type = EventType::Event_Combine;
			event.m_subType = JCommonAsyncEvent::SubType::SubType_End;
			event.m_progress = 1;
			this->Notify(&event);
		});
	th->start();
	QObject::connect(th, &QThread::finished, th, &QObject::deleteLater);
	return true;
}

JWeChatDBDecryptManager::JWeChatDBDecryptManager()
{
	static const QList<QPair<WeChatMajorVersion, const wchar_t*>> s_versionNameMap = []()
		{
			QList<QPair<WeChatMajorVersion, const wchar_t*>> versionNameMap;
			versionNameMap.emplace_back(WeChatMajorVersion::Version_3, L"WeChat.exe");
			versionNameMap.emplace_back(WeChatMajorVersion::Version_4, L"Weixin.exe");
			return versionNameMap;
		}();
	for (const auto& version : s_versionNameMap)
	{
		if (junuobase::utils::GetProcessIdByName(version.second) > 0)
		{
			m_majorVersion = version.first;
			break;
		}
	}
	m_processReader.reset(JWeChatProcessReaderFactory::createInstance(m_majorVersion));
}

QString JWeChatDBDecryptManager::getFinalDBFileName() const
{
	return m_finalDBFileName;
}
