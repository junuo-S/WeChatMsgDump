#pragma once

#include <QWidget>

class DecryptingPage : public QWidget
{
	Q_OBJECT
public:
	DecryptingPage(QWidget* parent);
	~DecryptingPage();
	void onBeginDecrypt(int totalCount);
	void onDecryptDoneOneFile(bool isSuccess);
	void onDecryptFinished();

signals:
	void sigReDecrypt();

private:
	struct Data;
	QScopedPointer<Data> data;
};
