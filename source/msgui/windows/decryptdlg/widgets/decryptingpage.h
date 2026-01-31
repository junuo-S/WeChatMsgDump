#pragma once

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include <global_interface.h>
class JunuoWaterProgressBar;
class JunuoBaseButton;

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

	void initUI();
	void initStyle();

	JunuoWaterProgressBar* m_progressBar = nullptr;
	QVBoxLayout* m_mainVLayout = nullptr;
	QLabel* m_tipLabel = nullptr;
	QHBoxLayout* m_buttonHLayout = nullptr;
	JunuoBaseButton* m_beginViewButton = nullptr;
	JunuoBaseButton* m_reDecryptButton = nullptr;
};
