#pragma once

#include <QWidget>

class DecryptorWapper;

class DecryptingPage : public QWidget
{
	Q_OBJECT
public:
	DecryptingPage(DecryptorWapper* wapper, QWidget* parent);
	~DecryptingPage();
	void startWork();

protected:
	void showEvent(QShowEvent* event) override;

signals:
	void sigReDecrypt();
	void sigBeginMsgView();

private:
	void onBeginDecrypt(int totalCount);
	void onDecryptDoneOneFile(bool isSuccess);
	void onDecryptFinished();
	void onCombineFinished(bool isSuccess);
	void onCombineOneFinished(bool isSuccess);
	void onCombineStarted(int totalCount);

	struct Data;
	QScopedPointer<Data> data;
};
