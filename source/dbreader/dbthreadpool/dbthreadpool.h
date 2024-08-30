#pragma once

#ifndef DBREADER_EXPORT

#ifdef _DBREADER_MODULE_
#define DBREADER_EXPORT __declspec(dllexport)
#else
#define DBREADER_EXPORT __declspec(dllimport)
#endif // _DBREADER_MODULE_

#endif // DBREADER_EXPORT

#include <QObject>
#include <memory>

class DBREADER_EXPORT JunuoDbThreadPool : public QObject
{
	Q_OBJECT
public:
	JunuoDbThreadPool(const QString& dbName, int maxCount = -1, QObject* parent = nullptr);
	~JunuoDbThreadPool();
	void executeQuery(const QString& sql, QObject* receiver, const char* method);

private:
	void onQueryFinished();
	struct Data;
	std::unique_ptr<Data> data = nullptr;
	friend class JunuoWorkerPool;
};
