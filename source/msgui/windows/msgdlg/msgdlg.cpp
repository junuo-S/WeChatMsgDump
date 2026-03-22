#include "stdafx.h"

#include "msgdlg.h"

#include <QHBoxLayout>
#include <QFile>
#include <QSplitter>

#include "msgapplication.h"
#include "utils/utils.h"
#include "leftwidgets/verticalnavigationbar.h"
#include "middlewidgets/middlepage.h"
#include "rightwidgets/chatpage.h"

#include "defines.h"

WechatMsgDialog::WechatMsgDialog(QWidget* parent /*= nullptr*/)
	: JunuoFrameLessWidget(parent)
{
	setWindowIcon(QIcon(":/icon_svg/wxchat.svg"));
}

WechatMsgDialog::~WechatMsgDialog()
{
}

void WechatMsgDialog::startWork()
{
	initUI();
	ComPtr<IJCoreApplication> coreApp = msgApp ? msgApp->GetCoreApplication() : nullptr;
	if (coreApp)
		m_spMsgViewMgr = coreApp->GetMsgViewManager();
	if (!m_spMsgViewMgr)
		return;
	attachTo(m_spMsgViewMgr.Get());
	m_spMsgViewMgr->StartQueryAllStrTalker();
	show();
	m_verticalNavigationBar->startWork();
	turnToPage(m_currentPage);
}

void WechatMsgDialog::onCurrentPageChanged(unsigned int index)
{
	m_currentPage = static_cast<WechatPage>(index);
	turnToPage(m_currentPage);
}

void WechatMsgDialog::initUI()
{
	m_shellLayout = new QHBoxLayout(this);
	m_shellLayout->setContentsMargins(DPI(10), DPI(10), DPI(10), DPI(10));
	m_mainWidget = new QWidget(this);
	m_mainWidget->setObjectName("mainWidget");
	m_shellLayout->addWidget(m_mainWidget);
	m_mainHLayout = new QHBoxLayout(m_mainWidget);
	m_mainHLayout->setContentsMargins(0, 0, 0, 0);
	m_mainHLayout->setSpacing(0);
	m_verticalNavigationBar = new VerticalNavigationBar(this);
	m_mainHLayout->addWidget(m_verticalNavigationBar);
	WechatMsgDialog::connect(m_verticalNavigationBar, &VerticalNavigationBar::sigCurrentPageChanged, this, &WechatMsgDialog::onCurrentPageChanged);

	m_splitter = new QSplitter(Qt::Horizontal, this);
	m_splitter->setChildrenCollapsible(false);
	m_middlePage = new MiddlePage(m_splitter);
	m_middlePage->setMaximumWidth(DPI(358));
	m_splitter->addWidget(m_middlePage);
	m_chatPage = new ChatPage(m_splitter);
	m_splitter->addWidget(m_chatPage);
	WechatMsgDialog::connect(m_middlePage, &MiddlePage::sigSessionClicked, m_chatPage, &ChatPage::setCurrentChatTalker);

	m_mainHLayout->addWidget(m_splitter);
	QFile qssFile(":/stylesheet/wechatmsgdialog.qss");
	if (qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(qssFile.readAll());
	}
	qssFile.close();
	setMinimumSize(DPI(1080), DPI(780));
}

void WechatMsgDialog::turnToPage(WechatPage page)
{
	m_verticalNavigationBar->setCurrentPage(page);
}

STDMETHODIMP_(bool) WechatMsgDialog::OnCoreEvent(IJCoreEvent* event)
{
	if (!event || event->Type() != EventType::Event_MsgView)
		return false;
	JMsgViewAsyncEvent* asyncEvent = dynamic_cast<JMsgViewAsyncEvent*>(event);
	if (!asyncEvent || asyncEvent->m_op != MsgViewOpType::Op_QueryAllStrTalker)
		return false;
	const QStringList talkerList = asyncEvent->m_extraData.value("talkerList").toStringList();
	QMetaObject::invokeMethod(this, "onQueryAllStrTalkerFinished", Qt::QueuedConnection, Q_ARG(QStringList, talkerList));
	return true;
}

Q_INVOKABLE void WechatMsgDialog::onQueryAllStrTalkerFinished(const QStringList& talkerList)
{
	for (const QString& wxid : talkerList)
	{
		if (!wxid.isEmpty())
			m_middlePage->addSessionCard(wxid);
	}
}
