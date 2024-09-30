#include "decryptorwapper.h"

#include <QThreadPool>

#include "../databus/databus.h"

#include "defines.h"
#include "dbdecryptor/wxmemoryreader.h"
#include "dbdecryptor/wxdbdecryptor.h"
#include "dbdecryptor/wxdbcombiner.h"

DecryptorWapper::DecryptorWapper(QObject* parent)
	: QObject(parent)
{

}

DecryptorWapper::~DecryptorWapper()
{

}

void DecryptorWapper::readMemory()
{
	QThreadPool::globalInstance()->start([this]()
		{
			WxMemoryReader memoryReader;
			bool suc = memoryReader.reRead();
			DATA_BUS_INSTANCE->setMemoryReadSuc(suc);
			DATA_BUS_INSTANCE->setWxVersion(QString::fromStdString(memoryReader.getWxVersion()));
			DATA_BUS_INSTANCE->setWxExePath(QString::fromStdWString(memoryReader.getWxExePath()));
			DATA_BUS_INSTANCE->setWxPhoneNumber(QString::fromStdString(memoryReader.getWxPhoneNumber()));
			DATA_BUS_INSTANCE->setWxUserName(QString::fromStdWString(memoryReader.getWxUserName()));
			DATA_BUS_INSTANCE->setWxNumber(QString::fromStdString(memoryReader.getWxNumber()));
			DATA_BUS_INSTANCE->setSecretKey(QString::fromStdString(memoryReader.getSecretKey()));
			DATA_BUS_INSTANCE->setWxid(QString::fromStdString(memoryReader.getWxid()));
			DATA_BUS_INSTANCE->setWxDataPath(QString::fromStdWString(memoryReader.getWxDataPath()));
			DATA_BUS_INSTANCE->setWxProcessId(memoryReader.getWxProcessId());
			if (suc)
				DATA_BUS_INSTANCE->autoSetDecryptPath();
			emit sigReadMemoryFinished(suc);
		});
}

void DecryptorWapper::decryptAndCombine()
{
	QThread* th = QThread::create([this]()
		{
			WxDBDecryptor decryptor({ WeChatDbType::_MSG, WeChatDbType::MediaMSG, WeChatDbType::MicroMsg, WeChatDbType::OpenIMContact, WeChatDbType::OpenIMMedia, WeChatDbType::OpenIMMsg }, DATA_BUS_INSTANCE->getWxDataPath(), DATA_BUS_INSTANCE->getDecryptOutputPath(), DATA_BUS_INSTANCE->getSecretKey());
			if (!decryptor.beforeDecrypt())
			{
				emit sigDecryptFailed();
				return;
			}
			connect(&decryptor, &WxDBDecryptor::sigUpdateProgress, this, &DecryptorWapper::sigUpdateProgress);
			connect(&decryptor, &WxDBDecryptor::sigDecryptFinished, this, &DecryptorWapper::sigDecryptFinished);
			connect(&decryptor, &WxDBDecryptor::sigDecryptFailed, this, &DecryptorWapper::sigDecryptFailed);
			decryptor.beginDecrypt();
			QStringList decryptedDbFiles = decryptor.getOutputDBFiles();
			if (decryptedDbFiles.isEmpty())
			{
				emit sigDecryptFailed();
				return;
			}

			WxDBCombiner combiner(decryptedDbFiles, DATA_BUS_INSTANCE->getMergedDbFilePath());
			connect(&combiner, &WxDBCombiner::sigUpdateProgress, this, &DecryptorWapper::sigUpdateProgress);
			connect(&combiner, &WxDBCombiner::sigCombineFinished, this, &DecryptorWapper::sigCombineFinished);
			connect(&combiner, &WxDBCombiner::sigCombineFailed, this, &DecryptorWapper::sigCombineFailed);
			combiner.beginCombine();
		});
	connect(th, &QThread::finished, th, &QObject::deleteLater);
	th->start();
}
