#include "msgmanager.h"

#include <QTimer>
#include <QStandardPaths>
#include <QFile>
#include <QMessageBox>

#include "windows/decryptdlg/decryptdlg.h"
#include "dbdecryptor/wxmemoryreader/wxmemoryreader.h"
#include "dbdecryptor/threads/wxmemorythread.h"
#include "dbdecryptor/threads/wxdbdecryptthread.h"
#include "dbreader/wechatdbreader.h"

MsgManager::MsgManager()
	: m_outputPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
{
	m_decryptDialog = new DecryptDialog(nullptr);
	connect(m_decryptDialog, &DecryptDialog::sigRefresh, this, &MsgManager::startWork);
	connect(m_decryptDialog, &DecryptDialog::sigStartDecrypt, this, &MsgManager::onStartDecrypt);
	connect(m_decryptDialog, &DecryptDialog::sigBeginMsgView, this, &MsgManager::onBeginMsgView);
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

void MsgManager::setInputPath(const QString& inputPath)
{
	m_inputPath = inputPath;
}

QString MsgManager::getInputPath() const
{
	return m_inputPath;
}

void MsgManager::setOutputPath(const QString& outputPath)
{
	m_outputPath = outputPath;
}

QString MsgManager::getOutputPath() const
{
	return m_outputPath;
}

void MsgManager::startReadWxMemory()
{
	m_wxReadThread = new WxMemoryReadThread(this);
	connect(m_wxReadThread, &WxMemoryReadThread::sigReadMemoryFinished, this, &MsgManager::onWxProcessDetectFinished);
	m_wxReadThread->start();
}

void MsgManager::onWxProcessDetectFinished(bool isSuccess)
{
	if (auto sender = qobject_cast<WxMemoryReadThread*>(this->sender()))
		sender->deleteLater();
	if (isSuccess)
		m_mergedDBPath = m_outputPath % "/" % QString::fromStdString(WxMemoryReader::instance()->getWxids().at(0)) % "/merge_db.db";
	QTimer::singleShot(3000, m_decryptDialog, &DecryptDialog::gotoWxProcessListPage);
}

void MsgManager::onStartDecrypt()
{
	m_wxDbDecryptThread = new WxDBDecryptThread(m_inputPath, m_outputPath, this);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigBeginDecrypt, m_decryptDialog, &DecryptDialog::onBeginDecrypt);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptDoneOneFile, m_decryptDialog, &DecryptDialog::onDecryptDoneOneFile);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptFinished, m_decryptDialog, &DecryptDialog::onDecryptFinished);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigCombineStarted, m_decryptDialog, &DecryptDialog::onCombineStarted);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigCombineOneFinished, m_decryptDialog, &DecryptDialog::onCombineOneFinished);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigCombineFinished, m_decryptDialog, &DecryptDialog::onCombineFinished);
	connect(m_wxDbDecryptThread, &QThread::finished, this, &MsgManager::onDecryptFinished);
	m_wxDbDecryptThread->start();
}

void MsgManager::onDecryptFinished()
{
	if (auto sender = qobject_cast<WxDBDecryptThread*>(this->sender()))
	{
		m_mergedDBPath = sender->getMergedDBPath();
		sender->deleteLater();
	}
	createWechatDbReader();
}

void MsgManager::onBeginMsgView()
{
	createWechatDbReader();
	if (!m_wechatDbReader)
	{
		QMessageBox::about(m_decryptDialog, tr("error"), tr("result don't exists"));
		return;
	}
}

void MsgManager::createWechatDbReader()
{
	if (!m_wechatDbReader && QFile::exists(m_mergedDBPath))
	{
		m_wechatDbReader = new WechatDbReader(m_mergedDBPath, this);
	}
}

