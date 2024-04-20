#include "msgmanager.h"

#include "windows/decryptdlg/decryptdlg.h"

MsgManager::MsgManager()
{
	m_decryptDialog = new DecryptDialog(nullptr);
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
	}
}

