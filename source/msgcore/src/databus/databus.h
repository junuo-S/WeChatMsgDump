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

protected:
	DataBus();

private:
	static DataBus* s_instance;
	QVariantMap m_wxInfo;
	bool m_memoryReadSuc = false;
};

#define DATA_BUS_INSTANCE DataBus::instance()
