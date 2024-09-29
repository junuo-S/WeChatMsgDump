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
	void onCombineFinished(bool isSuccess);
	void onCombineOneFinished(bool isSuccess);
	void onCombineStarted(int totalCount);

protected:
	void showEvent(QShowEvent* event) override;

signals:
	void sigReDecrypt();
	void sigBeginMsgView();

private:
	struct Data;
	QScopedPointer<Data> data;
};
