#pragma once

#include <QObject>

class DecryptDialog;
class WxMemoryReadThread;

class MsgManager : public QObject
{
	Q_OBJECT
public:
	MsgManager();
	~MsgManager();
	void startWork();

private:
	void startReadWxMemory();
	void onWxProcessDetectFinished(bool isSuccess);

	DecryptDialog* m_decryptDialog = nullptr;
	WxMemoryReadThread* m_wxReadThread = nullptr;
};
