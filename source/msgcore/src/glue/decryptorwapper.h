#pragma once

#ifndef MSGCORE_EXPORT
#	ifdef MSGCORE_MODULE
#		define MSGCORE_EXPORT __declspec(dllexport)
#	else
#		define MSGCORE_EXPORT __declspec(dllimport)
#	endif
#endif

#include <QObject>

class MSGCORE_EXPORT DecryptorWapper : public QObject
{
	Q_OBJECT
public:
	DecryptorWapper(QObject* parent);
	~DecryptorWapper();
	void readMemory();
	void decryptAndCombine();

signals:
	void sigReadMemoryFinished(bool isSuccess);
	void sigUpdateProgress(int current, int total);
	void sigDecryptFinished();
	void sigDecryptFailed();
	void sigCombineFinished();
	void sigCombineFailed();
};
