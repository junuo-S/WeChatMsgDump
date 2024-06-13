#include "msgmanager.h"

#include <QTimer>

#include "windows/decryptdlg/decryptdlg.h"
#include "dbdecryptor/wxmemoryreader/wxmemoryreader.h"
#include "dbdecryptor/threads/wxmemorythread.h"
#include "dbdecryptor/threads/wxdbdecryptthread.h"

MsgManager::MsgManager()
{
	m_decryptDialog = new DecryptDialog(nullptr);
	connect(m_decryptDialog, &DecryptDialog::sigRefresh, this, &MsgManager::startWork);
	connect(m_decryptDialog, &DecryptDialog::sigStartDecrypt, this, &MsgManager::onStartDecrypt);
}

MsgManager::~MsgManager()
{
	delete m_decryptDialog;
}

void MsgManager::startWork()
{
	if (m_decryptDialog)
	{
		m_decryptDialog->gotoLoadingPage();
		m_decryptDialog->show();
		startReadWxMemory();
	}
}

void MsgManager::startReadWxMemory()
{
	m_wxReadThread = new WxMemoryReadThread(this);
	connect(m_wxReadThread, &WxMemoryReadThread::sigReadMemoryFinished, this, &MsgManager::onWxProcessDetectFinished);
	m_wxReadThread->start();
}

void MsgManager::onWxProcessDetectFinished(bool isSuccess)
{
	auto sender = qobject_cast<WxMemoryReadThread*>(this->sender());
	disconnect(sender, &WxMemoryReadThread::sigReadMemoryFinished, this, &MsgManager::onWxProcessDetectFinished);
	delete sender;
	QTimer::singleShot(3000, m_decryptDialog, &DecryptDialog::gotoWxProcessListPage);
}

void MsgManager::onStartDecrypt()
{
	m_wxDbDecryptThread = new WxDBDecryptThread(this);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigBeginDecrypt, m_decryptDialog, &DecryptDialog::onBeginDecrypt);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptDoneOneFile, m_decryptDialog, &DecryptDialog::onDecryptDoneOneFile);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptFinished, m_decryptDialog, &DecryptDialog::onDecryptFinished);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptFinished, this, &MsgManager::onDecryptFinished);
	m_wxDbDecryptThread->start();
}

void MsgManager::onDecryptFinished()
{
	auto sender = qobject_cast<WxDBDecryptThread*>(this->sender());
	disconnect(m_wxDbDecryptThread, &WxDBDecryptThread::sigBeginDecrypt, m_decryptDialog, &DecryptDialog::onBeginDecrypt);
	disconnect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptDoneOneFile, m_decryptDialog, &DecryptDialog::onDecryptDoneOneFile);
	disconnect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptFinished, m_decryptDialog, &DecryptDialog::onDecryptFinished);
	disconnect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptFinished, this, &MsgManager::onDecryptFinished);
	delete sender;
}

