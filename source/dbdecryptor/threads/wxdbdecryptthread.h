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
	WxDBDecryptThread(const QString& inputPath, const QString& outputPath, QObject* parent = nullptr);
	void run() override;
	QString getMergedDBPath() const;

signals:
	void sigBeginDecrypt(int totalCount);
	void sigDecryptDoneOneFile(bool isSuccess);
	void sigDecryptFinished();
	void sigCombineFinished(bool isSuccess);
	void sigCombineOneFinished(bool isSuccess);
	void sigCombineStarted(int);

private:
	QString m_inputPath;
	QString m_outputPath;
	QString m_mergedDBPath;
};
