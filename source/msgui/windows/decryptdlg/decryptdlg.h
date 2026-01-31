#pragma once

#include <QWidget>
#include <QStackedLayout>

#include "junuoui/customwidget/junuoframelesswidget.h"
class LoadingPage;
class WxProcessListPage;
class DecryptingPage;
class JunuoBaseTitleBar;

class DecryptDialog : public JunuoFrameLessWidget
{
	Q_OBJECT
public:
	DecryptDialog(QWidget* parent);
	~DecryptDialog();
	void startWork();

signals:
	void sigBeginMsgView();

private:
	void gotoLoadingPage();
	void gotoWxProcessListPage();
	void gotoDecryptingPage();
	void onPageChanged(int index);

	void initUI();

	QWidget* m_mainWidget = nullptr;
	LoadingPage* m_loadingPage = nullptr;
	WxProcessListPage* m_wxProcessListPage = nullptr;
	DecryptingPage* m_decryptingPage = nullptr;
	QStackedLayout* m_stackedLayout = nullptr;
	JunuoBaseTitleBar* m_titleBar = nullptr;
};
