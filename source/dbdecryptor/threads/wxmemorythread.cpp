#include "wxmemorythread.h"

#include "../wxmemoryreader/wxmemoryreader.h"

WxMemoryReadThread::WxMemoryReadThread(QObject* parent)
	: QThread(parent)
{

}

void WxMemoryReadThread::run()
{
	bool success = WxMemoryReader::instance()->reset();
	emit sigReadMemoryFinished(success);
}

