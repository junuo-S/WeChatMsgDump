#include "wxdbdecryptthread.h"

#include "defines.h"
#include "../wxmemoryreader/wxmemoryreader.h"
#include "../wxdbdecryptor/wxdbdecryptor.h"
#include "../wxdbcombiner/wxdbcombiner.h"

WxDBDecryptThread::WxDBDecryptThread(QObject* parent /*= nullptr*/)
	: QThread(parent)
{

}

void WxDBDecryptThread::run()
{
	WxDBDecryptor decryptor({ WeChatDbType::_MSG, WeChatDbType::MediaMSG, WeChatDbType::MicroMsg, WeChatDbType::OpenIMContact, WeChatDbType::OpenIMMedia, WeChatDbType::OpenIMMsg });
	connect(&decryptor, &WxDBDecryptor::sigDecryptDoneOneFile, this, &WxDBDecryptThread::sigDecryptDoneOneFile);
	connect(&decryptor, &WxDBDecryptor::sigDecryptAllDone, this, &WxDBDecryptThread::sigDecryptFinished);
	if (!decryptor.beforeDecrypt())
		return;
	size_t totalCount = decryptor.getTotalDBFileCount();
	emit sigBeginDecrypt(totalCount);
	decryptor.beginDecrypt();
	WxDBCombiner combiner(decryptor.getOutputDBFiles());
	connect(&combiner, &WxDBCombiner::sigCombineStarted, this, &WxDBDecryptThread::sigCombineStarted);
	connect(&combiner, &WxDBCombiner::sigCombineOneFinished, this, &WxDBDecryptThread::sigCombineOneFinished);
	connect(&combiner, &WxDBCombiner::sigCombineFinished, this, &WxDBDecryptThread::sigCombineFinished);
	combiner.beginCombine();
}

