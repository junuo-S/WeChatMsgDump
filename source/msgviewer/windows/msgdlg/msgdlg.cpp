#include "stdafx.h"

#include "msgdlg.h"

#include <QHBoxLayout>
#include <QFile>

#include "leftwidgets/verticalnavigationbar.h"

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
		verticalNavigationBar = new VerticalNavigationBar(q);
		mainHLayout->addWidget(verticalNavigationBar);
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

void WechatMsgDialog::initUI()
{
	data->initUI();
	setMinimumSize(DPI(680), DPI(560));
}

