#pragma once

#include <QApplication>

#include <global_interface.h>

class MsgApplication : public QApplication
{
	Q_OBJECT
public:
	MsgApplication(int& argc, char** argv);
	void init(IJCoreEntry* pCoreEntry);
	IJCoreApplication* GetCoreApplication();

private:
	ComPtr<IJCoreEntry> m_spCoreEntry;
};

#define msgApp qobject_cast<MsgApplication*>(QCoreApplication::instance())
