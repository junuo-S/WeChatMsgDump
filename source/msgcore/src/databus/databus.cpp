﻿#include "databus.h"

#include <QStandardPaths>
#include <QDir>

#include "defines.h"

#include "../interface/interfacedecl.h"

void DataBus::notifyHeadImage(const QString& wxid)
{
	if (!m_headImageObservers.contains(wxid))
		return;
	QPixmap* pixmap = m_headImgCache.object(wxid);
	if (!pixmap)
		return;
	for (IHeadImageObserver* observer : m_headImageObservers.value(wxid))
	{
		observer->setHeadImage(*pixmap);
	}
}

DataBus* DataBus::s_instance = new DataBus;

DataBus::~DataBus()
{

}

DataBus* DataBus::instance()
{
	return s_instance;
}

void DataBus::setMemoryReadSuc(bool suc)
{
	m_memoryReadSuc = suc;
}

bool DataBus::getMemoryReadSuc() const
{
	return m_memoryReadSuc;
}

void DataBus::setWxVersion(const QString& version)
{
	m_wxInfo.insert(STR_WX_VERSION, version);
}

QString DataBus::getWxVersion() const
{
	return m_wxInfo.value(STR_WX_VERSION).toString();
}

void DataBus::setWxExePath(const QString& path)
{
	m_wxInfo.insert(STR_WX_EXE_PATH, path);
}

QString DataBus::getWxExePath() const
{
	return m_wxInfo.value(STR_WX_EXE_PATH).toString();
}

void DataBus::setWxPhoneNumber(const QString& number)
{
	m_wxInfo.insert(STR_WX_PHONE_NUMBER, number);
}

QString DataBus::getWxPhoneNumber() const
{
	return m_wxInfo.value(STR_WX_PHONE_NUMBER).toString();
}

void DataBus::setWxUserName(const QString& userName)
{
	m_wxInfo.insert(STR_WX_USER_NAME, userName);
}

QString DataBus::getWxUserName() const
{
	return m_wxInfo.value(STR_WX_USER_NAME).toString();
}

void DataBus::setWxNumber(const QString& number)
{
	m_wxInfo.insert(STR_WX_NUMBER, number);
}

QString DataBus::getWxNumber() const
{
	return m_wxInfo.value(STR_WX_NUMBER).toString();
}

void DataBus::setSecretKey(const QString& secretKey)
{
	m_wxInfo.insert(STR_SECRET_KEY, secretKey);
}

QString DataBus::getSecretKey() const
{
	return m_wxInfo.value(STR_SECRET_KEY).toString();
}

void DataBus::setWxid(const QString& wxid)
{
	m_wxInfo.insert(STR_WXID, wxid);
}

QString DataBus::getWxid() const
{
	return m_wxInfo.value(STR_WXID).toString();
}

void DataBus::setWxDataPath(const QString& path)
{
	m_wxInfo.insert(STR_WX_DATA_PATH, path);
}

QString DataBus::getWxDataPath() const
{
	return m_wxInfo.value(STR_WX_DATA_PATH).toString();
}

void DataBus::setWxProcessId(quint64 id)
{
	m_wxInfo.insert(STR_WX_PROCESS_ID, id);
}

quint64 DataBus::getWxProcessId() const
{
	return m_wxInfo.value(STR_WX_PROCESS_ID).toULongLong();
}

void DataBus::setDecryptOutputPath(const QString& path)
{
	m_decryptOutputPath = path;
}

QString DataBus::getDecryptOutputPath() const
{
	return m_decryptOutputPath;
}

void DataBus::setMergedDbFilePath(const QString& path)
{
	m_mergedDbFilePath = path;
}

QString DataBus::getMergedDbFilePath() const
{
	return m_mergedDbFilePath;
}

void DataBus::autoSetDecryptPath()
{
	m_decryptOutputPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	m_mergedDbFilePath = m_decryptOutputPath + QDir::separator() + getWxid() + QDir::separator() + "merged_db.db";
}

void DataBus::addHeadImage(const QString& wxid, QPixmap* pixmap, bool notifyAll/* = true*/)
{
	m_headImgCache.insert(wxid, pixmap);
	if (notifyAll)
		notifyHeadImage(wxid);
}

QPixmap* DataBus::getHeadImage(const QString& wxid) const
{
	return m_headImgCache.object(wxid);
}

void DataBus::attachHeadImageObserver(const QString& wxid, IHeadImageObserver* observer)
{
	if (m_headImageObservers.contains(wxid) && !m_headImageObservers[wxid].contains(observer))
	{
		m_headImageObservers[wxid].append(observer);
	}
	else
	{
		m_headImageObservers.insert(wxid, { observer });
	}
}

void DataBus::detachHeadImageObserver(const QString& wxid, IHeadImageObserver* observer)
{
	if (m_headImageObservers.contains(wxid))
		m_headImageObservers[wxid].removeOne(observer);
}

DataBus::DataBus()
{	
}

