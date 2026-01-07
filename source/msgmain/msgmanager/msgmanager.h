#pragma once

#include <QObject>

class DecryptDialog;
class WechatDbReader;
class WechatMsgDialog;

class MsgManager : public QObject
{
	Q_OBJECT
public:
	MsgManager();
	~MsgManager();
	void startWork();

private:
	void onBeginMsgViewer();

	QScopedPointer<DecryptDialog> m_decryptDialog;
	QScopedPointer<WechatMsgDialog> m_msgDialog;
};
