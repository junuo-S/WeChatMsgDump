#pragma once

class DecryptDialog;

class MsgManager
{
public:
	MsgManager();
	~MsgManager();
	void startWork();

private:
	DecryptDialog* m_decryptDialog = nullptr;
};
