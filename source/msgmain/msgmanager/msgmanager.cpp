#include "msgmanager.h"

#include <QTimer>
#include <QStandardPaths>
#include <QFile>
#include <QMessageBox>

#include "windows/decryptdlg/decryptdlg.h"
#include "windows/msgdlg/msgdlg.h"

#include "dbdecryptor/wxmemoryreader/wxmemoryreader.h"
#include "dbdecryptor/threads/wxmemorythread.h"
#include "dbdecryptor/threads/wxdbdecryptthread.h"
#include "dbreader/wechatdbreader.h"

MsgManager* MsgManager::s_msgManager = nullptr;

MsgManager::MsgManager()
	: m_outputPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation))
{
	m_decryptDialog.reset(new DecryptDialog(nullptr));
	connect(m_decryptDialog.get(), &DecryptDialog::sigRefresh, this, &MsgManager::startWork);
	connect(m_decryptDialog.get(), &DecryptDialog::sigStartDecrypt, this, &MsgManager::onStartDecrypt);
	connect(m_decryptDialog.get(), &DecryptDialog::sigBeginMsgView, this, &MsgManager::onBeginMsgView);
	m_msgDialog.reset(new WechatMsgDialog(nullptr));
}

MsgManager::~MsgManager()
{
	
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

MsgManager* MsgManager::instance()
{
	if (!s_msgManager)
		s_msgManager = new MsgManager;
	return s_msgManager;
}

QString MsgManager::getCurrentUserWxid() const
{
	return QString::fromStdString(WxMemoryReader::instance()->getWxids().at(0));
}

WechatDbReader* MsgManager::getWechatDbReader() const
{
	return m_wechatDbReader;
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
	QTimer::singleShot(3000, m_decryptDialog.get(), &DecryptDialog::gotoWxProcessListPage);
}

void MsgManager::onStartDecrypt()
{
	m_wxDbDecryptThread = new WxDBDecryptThread(m_inputPath, m_outputPath, this);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigBeginDecrypt, m_decryptDialog.get(), &DecryptDialog::onBeginDecrypt);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptDoneOneFile, m_decryptDialog.get(), &DecryptDialog::onDecryptDoneOneFile);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigDecryptFinished, m_decryptDialog.get(), &DecryptDialog::onDecryptFinished);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigCombineStarted, m_decryptDialog.get(), &DecryptDialog::onCombineStarted);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigCombineOneFinished, m_decryptDialog.get(), &DecryptDialog::onCombineOneFinished);
	connect(m_wxDbDecryptThread, &WxDBDecryptThread::sigCombineFinished, m_decryptDialog.get(), &DecryptDialog::onCombineFinished);
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
		QMessageBox::about(m_decryptDialog.get(), tr("error"), tr("result don't exists"));
		return;
	}
	m_decryptDialog->close();
	m_msgDialog->startWork();
}

void MsgManager::createWechatDbReader()
{
	if (!m_wechatDbReader && QFile::exists(m_mergedDBPath))
	{
		m_wechatDbReader = new WechatDbReader(m_mergedDBPath, this);
	}
}

