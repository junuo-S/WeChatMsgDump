#pragma once

#ifndef MSGCORE_EXPORT
#	ifdef MSGCORE_MODULE
#		define MSGCORE_EXPORT __declspec(dllexport)
#	else
#		define MSGCORE_EXPORT __declspec(dllimport)
#	endif
#endif

#include <QObject>

class DecryptorWapper : public QObject
{
	Q_OBJECT
public:
	DecryptorWapper(QObject* parent);
	~DecryptorWapper();
	void readMemory();

signals:
	void sigReadMemoryFinished(bool isSuccess);
};
