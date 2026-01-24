#pragma once

#include <QApplication>

#include <global_interface.h>

class DecryptDialog;
class WechatMsgDialog;

class MsgApplication : public QApplication
{
	Q_OBJECT
public:
	MsgApplication(int& argc, char** argv);
	~MsgApplication();
	void init(IJCoreEntry* pCoreEntry);
	IJCoreApplication* GetCoreApplication();
	void startWork();

private:
	void onBeginMsgViewer();

	ComPtr<IJCoreEntry> m_spCoreEntry;
	QScopedPointer<DecryptDialog> m_decryptDialog;
	QScopedPointer<WechatMsgDialog> m_msgDialog;
};

#define msgApp qobject_cast<MsgApplication*>(QCoreApplication::instance())
