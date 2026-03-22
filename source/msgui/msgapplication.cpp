#include "stdafx.h"

#include "msgapplication.h"

#include <QMessageBox>

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
	m_msgDialog.reset(new WechatMsgDialog(nullptr));
	connect(m_decryptDialog.get(), &DecryptDialog::sigBeginMsgView, this, &MsgApplication::onBeginMsgViewer);
}

ComPtr<IJCoreApplication> MsgApplication::GetCoreApplication()
{
	return m_spCoreEntry ? m_spCoreEntry->GetCoreApplication() : nullptr;
}

void MsgApplication::startWork()
{
	if (m_decryptDialog)
		m_decryptDialog->startWork();
}

void MsgApplication::onBeginMsgViewer()
{
	ComPtr<IJCoreApplication> coreApp = GetCoreApplication();
	ComPtr<IJMsgViewManager> msgViewMgr = coreApp ? coreApp->GetMsgViewManager() : nullptr;
	if (!coreApp || !msgViewMgr)
	{
		QMessageBox::about(m_decryptDialog.get(), tr("error"), tr("result don't exists"));
		return;
	}
	m_decryptDialog->close();
	if (m_msgDialog)
		m_msgDialog->startWork();
}

