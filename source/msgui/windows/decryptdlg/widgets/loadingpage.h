#pragma once

#include <QWidget>

#include <global_interface.h>

class LoadingPage : public QWidget, public IJCoreObserver
{
	Q_OBJECT
public:
	explicit LoadingPage(QWidget* parent = nullptr);
	~LoadingPage();
	void startWork();

signals:
	void sigLoadingFinished();

private:
	STDMETHODIMP_(bool) OnCoreEvent(IJCoreEvent* event) override;
	void onMemoryReadFinished();

	struct Data;
	QScopedPointer<Data> data;
};
