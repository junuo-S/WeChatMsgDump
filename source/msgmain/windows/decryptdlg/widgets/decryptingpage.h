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
	void onUpdateProgress(int current, int total);
	void onDecryptFailed();
	void onDecryptFinished();
	void onCombineFailed();
	void onCombineFinished();
	void disableButtons();
	void enableButtons();

	struct Data;
	QScopedPointer<Data> data;
};
