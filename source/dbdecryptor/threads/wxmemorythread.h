#pragma once

#ifndef DBDECRYPTOR_EXPORT

#ifdef _DB_DECRYPTOR_MODULE_
#define DBDECRYPTOR_EXPORT __declspec(dllexport)
#else
#define DBDECRYPTOR_EXPORT __declspec(dllimport)
#endif // _DB_DECRYPTOR_MODULE_

#endif // DBDECRYPTOR_EXPORT

#include <QThread>

class DBDECRYPTOR_EXPORT WxMemoryReadThread : public QThread
{
	Q_OBJECT
public:
	WxMemoryReadThread(QObject* parent);
	void run() override;
signals:
	void sigReadMemoryFinished(bool success);
};