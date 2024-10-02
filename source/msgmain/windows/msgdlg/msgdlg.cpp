#include "stdafx.h"

#include "msgdlg.h"

#include <QHBoxLayout>
#include <QFile>
#include <QSplitter>

#include "utils/utils.h"
#include "msgmanager/msgmanager.h"
#include "leftwidgets/verticalnavigationbar.h"
#include "middlewidgets/middlepage.h"

#include "defines.h"
#include "msgcore/databus/databus.h"

struct WechatMsgDialog::Data
{
	void initUI()
	{
		shellLayout = new QHBoxLayout(q);
		shellLayout->setContentsMargins(DPI(10), DPI(10), DPI(10), DPI(10));
		mainWidget = new QWidget(q);
		mainWidget->setObjectName("mainWidget");
		shellLayout->addWidget(mainWidget);
		mainHLayout = new QHBoxLayout(mainWidget);
		mainHLayout->setContentsMargins(0, 0, 0, 0);
		mainHLayout->setSpacing(0);
		verticalNavigationBar = new VerticalNavigationBar(q);
		mainHLayout->addWidget(verticalNavigationBar);
		WechatMsgDialog::connect(verticalNavigationBar, &VerticalNavigationBar::sigCurrentPageChanged, q, &WechatMsgDialog::onCurrentPageChanged);
		
		splitter = new QSplitter(Qt::Horizontal, q);
		middlePage = new MiddlePage(splitter);
		splitter->addWidget(middlePage);

		mainHLayout->addWidget(splitter);
		mainHLayout->addStretch();
		QFile qssFile(":/stylesheet/wechatmsgdialog.qss");
		if (qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			q->setStyleSheet(qssFile.readAll());
		}
		qssFile.close();
	}

	WechatMsgDialog* q = nullptr;
	QLayout* shellLayout = nullptr;
	QWidget* mainWidget = nullptr;
	QHBoxLayout* mainHLayout = nullptr;
	VerticalNavigationBar* verticalNavigationBar = nullptr;
	QSplitter* splitter = nullptr;
	MiddlePage* middlePage = nullptr;
	WechatPage currentPage = ChatPage;
};

WechatMsgDialog::WechatMsgDialog(QWidget* parent /*= nullptr*/)
	: JunuoFrameLessWidget(parent)
	, data(new Data)
{
	data->q = this;
	setWindowIcon(QIcon(":/icon_svg/wxchat.svg"));
}

WechatMsgDialog::~WechatMsgDialog()
{

}

void WechatMsgDialog::startWork()
{
	initUI();
	DATA_BUS_INSTANCE->requestAllStrTalker(this, "onSelectDistinctStrTalkerFinished");
	show();
	data->verticalNavigationBar->startWork();
	turnToPage(data->currentPage);
}

void WechatMsgDialog::onCurrentPageChanged(unsigned int index)
{
	data->currentPage = static_cast<WechatPage>(index);
	turnToPage(data->currentPage);
}

void WechatMsgDialog::initUI()
{
	data->initUI();
	setMinimumSize(DPI(900), DPI(780));
}

void WechatMsgDialog::turnToPage(WechatPage page)
{
	data->verticalNavigationBar->setCurrentPage(page);
}

Q_INVOKABLE void WechatMsgDialog::onSelectDistinctStrTalkerFinished(const QVariantList& result, const QVariant& context /*= QVariant()*/)
{
	for (const auto& record : result)
	{
		const QString wxid = record.toMap().value(STR_STRTALKER).toString();
		if (!wxid.isEmpty())
			data->middlePage->addSessionCard(wxid);
	}
}
