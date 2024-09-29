#include "decryptorwapper.h"

#include <QThreadPool>

#include "../databus/databus.h"

#include "dbdecryptor/wxmemoryreader/wxmemoryreader.h"

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
			emit sigReadMemoryFinished(suc);
		});
}
