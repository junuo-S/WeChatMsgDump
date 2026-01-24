#include "stdafx.h"

#include "msgapplication.h"

#include "windows/decryptdlg/decryptdlg.h"
#include "windows/msgdlg/msgdlg.h"

MsgApplication::MsgApplication(int& argc, char** argv)
	: QApplication(argc, argv)
{
}

MsgApplication::~MsgApplication()
{

}

void MsgApplication::init(IJCoreEntry* pCoreEntry)
{
	m_spCoreEntry = pCoreEntry;
	m_decryptDialog.reset(new DecryptDialog(nullptr));
	//m_msgDialog.reset(new WechatMsgDialog(nullptr));
	connect(m_decryptDialog.get(), &DecryptDialog::sigBeginMsgView, this, &MsgApplication::onBeginMsgViewer);
}

IJCoreApplication* MsgApplication::GetCoreApplication()
{
	return m_spCoreEntry->GetCoreApplication();
}

void MsgApplication::startWork()
{
	if (m_decryptDialog)
		m_decryptDialog->startWork();
}

void MsgApplication::onBeginMsgViewer()
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

