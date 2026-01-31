#pragma once

#include <QWidget>

#include <global_interface.h>

class QVBoxLayout;
class QLabel;
class JunuoNormalLoadingWidget;

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
	void initUI();

	QVBoxLayout* m_mainLayout = nullptr;
	QLabel* m_titleLabel = nullptr;
	JunuoNormalLoadingWidget* m_m_loadingWidget = nullptr;
	QLabel* m_loadingTextLabel = nullptr;
};
