#pragma once

#include <QWidget>

#include <global_interface.h>

class DecryptingPage : public QWidget, public IJCoreObserver
{
	Q_OBJECT
public:
	DecryptingPage(QWidget* parent);
	~DecryptingPage();
	void startWork();

protected:
	void showEvent(QShowEvent* event) override;

signals:
	void sigReDecrypt();
	void sigBeginMsgView();

private:
	STDMETHODIMP_(bool) OnCoreEvent(IJCoreEvent* event) override;
	Q_INVOKABLE void processCoreEvent(const JCommonAsyncEvent& event);
	void updateProgress(int current);
	void onDecryptFinished();
	void onDecryptFailed();
	void onCombineFinished();

	struct Data;
	QScopedPointer<Data> data;
};
