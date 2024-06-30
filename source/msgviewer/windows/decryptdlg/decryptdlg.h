#pragma once

#include <QWidget>
#include <QScopedPointer>

class DecryptDialog : public QWidget
{
	Q_OBJECT
public:
	DecryptDialog(QWidget* parent);
	~DecryptDialog();
	void gotoLoadingPage();
	void gotoWxProcessListPage();
	void gotoDecryptingPage();
	void onBeginDecrypt(size_t totalCount);
	void onDecryptDoneOneFile(bool isSuccess);
	void onDecryptFinished();
	void onCombineFinished(bool isSuccess);
	void onCombineOneFinished(bool isSuccess);
	void onCombineStarted(int totalCount);

signals:
	void sigRefresh();
	void sigStartDecrypt();
	void sigBeginMsgView();

private:
	void onPageChanged(int index);
	struct Data;
	QScopedPointer<Data> data;
};
