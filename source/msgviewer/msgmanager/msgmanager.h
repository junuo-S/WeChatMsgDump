﻿#pragma once

#include <QObject>
#include <QScopedPointer>

class DecryptDialog;
class WxMemoryReadThread;
class WxDBDecryptThread;
class WechatDbReader;
class WechatMsgDialog;

class MsgManager : public QObject
{
	Q_OBJECT
public:
	MsgManager();
	~MsgManager();
	void startWork();
	void setInputPath(const QString& inputPath);
	QString getInputPath() const;
	void setOutputPath(const QString& outputPath);
	QString getOutputPath() const;

private:
	void startReadWxMemory();
	void onWxProcessDetectFinished(bool isSuccess);
	void onStartDecrypt();
	void onDecryptFinished();
	void onBeginMsgView();
	void createWechatDbReader();

	QScopedPointer<DecryptDialog> m_decryptDialog;
	WxMemoryReadThread* m_wxReadThread = nullptr;
	WxDBDecryptThread* m_wxDbDecryptThread = nullptr;
	QString m_inputPath;
	QString m_outputPath;
	QString m_mergedDBPath;
	WechatDbReader* m_wechatDbReader = nullptr;
	QScopedPointer<WechatMsgDialog> m_msgDialog;
};
