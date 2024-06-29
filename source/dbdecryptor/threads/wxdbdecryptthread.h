#pragma once

#ifndef DBDECRYPTOR_EXPORT

#ifdef _DB_DECRYPTOR_MODULE_
#define DBDECRYPTOR_EXPORT __declspec(dllexport)
#else
#define DBDECRYPTOR_EXPORT __declspec(dllimport)
#endif // _DB_DECRYPTOR_MODULE_

#endif // DBDECRYPTOR_EXPORT

#include <QThread>

class DBDECRYPTOR_EXPORT WxDBDecryptThread : public QThread
{
	Q_OBJECT
public:
	WxDBDecryptThread(QObject* parent = nullptr);
	void run() override;

signals:
	void sigBeginDecrypt(int totalCount);
	void sigDecryptDoneOneFile(bool isSuccess);
	void sigDecryptFinished();
	void sigCombineFinished(bool isSuccess);
	void sigCombineOneFinished(bool isSuccess);
	void sigCombineStarted(int);
};
