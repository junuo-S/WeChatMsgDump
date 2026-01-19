#include "msgmanager.h"

#include <QMessageBox>

#include "windows/decryptdlg/decryptdlg.h"
#include "windows/msgdlg/msgdlg.h"

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
	// todo
	//if (!DATA_BUS_INSTANCE->createDbReader())
	//{
	//	QMessageBox::about(m_decryptDialog.get(), tr("error"), tr("result don't exists"));
	//	return;
	//}
	m_decryptDialog->close();
	//m_msgDialog->startWork();
}
