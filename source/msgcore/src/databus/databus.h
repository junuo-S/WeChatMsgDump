#pragma once

#ifndef MSGCORE_EXPORT
#	ifdef MSGCORE_MODULE
#		define MSGCORE_EXPORT __declspec(dllexport)
#	else
#		define MSGCORE_EXPORT __declspec(dllimport)
#	endif
#endif

#include <QObject>
#include <QVariantMap>
#include <QCache>
#include <QPixmap>

class IHeadImageObserver;
class WechatDbReader;

class MSGCORE_EXPORT DataBus : public QObject
{
	Q_OBJECT
public:
	~DataBus();
	static DataBus* instance();
	void setMemoryReadSuc(bool suc);
	bool getMemoryReadSuc() const;
	void setWxVersion(const QString& version);
	QString getWxVersion() const;
	void setWxExePath(const QString& path);
	QString getWxExePath() const;
	void setWxPhoneNumber(const QString& number);
	QString getWxPhoneNumber() const;
	void setWxUserName(const QString& userName);
	QString getWxUserName() const;
	void setWxNumber(const QString& number);
	QString getWxNumber() const;
	void setSecretKey(const QString& secretKey);
	QString getSecretKey() const;
	void setWxid(const QString& wxid);
	QString getWxid() const;
	void setWxDataPath(const QString& path);
	QString getWxDataPath() const;
	void setWxProcessId(quint64 id);
	quint64 getWxProcessId() const;
	void setDecryptOutputPath(const QString& path);
	QString getDecryptOutputPath() const;
	void setMergedDbFilePath(const QString& path);
	QString getMergedDbFilePath() const;
	void autoSetDecryptPath();
	bool createDbReader();
	void addHeadImage(const QString& wxid, QPixmap* pixmap, bool notifyAll = true);
	QPixmap* getHeadImage(const QString& wxid) const;
	void attachHeadImageObserver(const QString& wxid, IHeadImageObserver* observer);
	void detachHeadImageObserver(const QString& wxid, IHeadImageObserver* observer);
	void requestHeadImage(const QString& wxid, IHeadImageObserver* observer = nullptr);

protected:
	DataBus();

private:
	void notifyHeadImage(const QString& wxid);
	Q_INVOKABLE void onSelectHeadImageFinished(const QVariantList& result, const QVariant& context = QVariant());

	static DataBus* s_instance;
	QVariantMap m_wxInfo;
	bool m_memoryReadSuc = false;
	QString m_decryptOutputPath;
	QString m_mergedDbFilePath;
	QCache<QString, QPixmap> m_headImgCache;
	QHash<QString, QVector<IHeadImageObserver*>> m_headImageObservers;
	WechatDbReader* m_dbReader = nullptr;
};

#define DATA_BUS_INSTANCE DataBus::instance()
