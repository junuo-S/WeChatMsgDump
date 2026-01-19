#include "stdafx.h"

#include "msgapplication.h"

MsgApplication::MsgApplication(int& argc, char** argv)
	: QApplication(argc, argv)
{
}

void MsgApplication::init(IJCoreEntry* pCoreEntry)
{
	m_spCoreEntry = pCoreEntry;
}

IJCoreApplication* MsgApplication::GetCoreApplication()
{
	return m_spCoreEntry->GetCoreApplication();
}

