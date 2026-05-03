#pragma once

#include <functional>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QTimer>
#include <QWidget>

#include "junuoui/customwidget/junuoframelesswidget.h"
#include <global_interface.h>

#include "global.h"

class QLayout;
class QVBoxLayout;
class QHBoxLayout;
class QSplitter;
class JunuoBaseTitleBar;
class VerticalNavigationBar;
class MiddlePage;
class ChatPage;

class WechatMsgDialog : public JunuoFrameLessWidget, public IJCoreObserver
{
	Q_OBJECT
public:
	WechatMsgDialog(QWidget* parent = nullptr);
	~WechatMsgDialog();
	void startWork();
	STDMETHODIMP_(bool) OnCoreEvent(IJCoreEvent* event) override;

private:
	void onCurrentPageChanged(unsigned int index);
	void initUI();
	void turnToPage(WechatPage page);
	Q_INVOKABLE void onQueryAllStrTalkerFinished(const QStringList& talkerList);

	QLayout* m_shellLayout = nullptr;
	QWidget* m_mainWidget = nullptr;
	QVBoxLayout* m_mainVLayout = nullptr;
	JunuoBaseTitleBar* m_titleBar = nullptr;
	QWidget* m_contentWidget = nullptr;
	QHBoxLayout* m_mainHLayout = nullptr;
	VerticalNavigationBar* m_verticalNavigationBar = nullptr;
	QSplitter* m_splitter = nullptr;
	MiddlePage* m_middlePage = nullptr;
	ChatPage* m_chatPage = nullptr;
	WechatPage m_currentPage = Wechat_ChatPage;
	ComPtr<IJMsgViewManager> m_spMsgViewMgr;
};
