#include "wxdbdecryptthread.h"

#include "defines.h"
#include "../wxmemoryreader/wxmemoryreader.h"
#include "../wxdbdecryptor/wxdbdecryptor.h"

WxDBDecryptThread::WxDBDecryptThread(QObject* parent /*= nullptr*/)
	: QThread(parent)
{

}

void WxDBDecryptThread::run()
{
	WxDBDecryptor d(WeChatDbType::_MSG);
	d.beginDecrypt();
}

