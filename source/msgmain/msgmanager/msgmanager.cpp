#include "msgmanager.h"

#include <QMessageBox>
#include <QFile>

#include "windows/decryptdlg/decryptdlg.h"
//#include "windows/msgdlg/msgdlg.h"

#include "msgcore/databus/databus.h"

MsgManager::MsgManager()
{
	m_decryptDialog.reset(new DecryptDialog(nullptr));
	//m_msgDialog.reset(new WechatMsgDialog(nullptr));
	connect(m_decryptDialog.get(), &DecryptDialog::sigBeginMsgView, this, &MsgManager::onBeginMsgViewer);
}

MsgManager::~MsgManager()
{
	
}

void MsgManager::startWork()
{
	if (m_decryptDialog)
	{
		m_decryptDialog->startWork();
	}
}

void MsgManager::onBeginMsgViewer()
{
	if (!QFile::exists(DATA_BUS_INSTANCE->getMergedDbFilePath()))
	{
		QMessageBox::about(m_decryptDialog.get(), tr("error"), tr("result don't exists"));
	}
}
