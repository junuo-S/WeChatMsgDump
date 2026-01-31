#include "stdafx.h"

#include "decryptdlg.h"

#include <QStackedLayout>
#include <QFile>

#include "widgets/loadingpage.h"
#include "widgets/wxprocesslistpage.h"
#include "widgets/decryptingpage.h"

#include "junuoui/customwidget/junuobasetitlebar.h"

DecryptDialog::DecryptDialog(QWidget* parent)
	: JunuoFrameLessWidget(parent)
{
	initUI();
	setMainWidget(m_mainWidget);
	setWindowIcon(QIcon(":/icon_svg/wxchat.svg"));
}

DecryptDialog::~DecryptDialog()
{

}

void DecryptDialog::startWork()
{
	gotoLoadingPage();
	show();
}

void DecryptDialog::initUI()
{
	m_mainWidget = new QWidget(this);
	m_mainWidget->setObjectName("mainWidget");

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->setContentsMargins(DPI(10), DPI(10), DPI(10), DPI(10));

	m_titleBar = new JunuoBaseTitleBar(QPixmap(":/icon_svg/wxchat.svg"), DecryptDialog::tr("wechat db decrypt"), this);
	m_titleBar->setObjectName("titleBar");
	m_titleBar->setTitleIconSize(DPI_SIZE(18, 18));
	setTitleBar(m_titleBar);

	vLayout->addWidget(m_mainWidget);

	QVBoxLayout* mainVLayout = new QVBoxLayout(m_mainWidget);
	mainVLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->setSpacing(0);
	mainVLayout->addWidget(m_titleBar);

	m_stackedLayout = new QStackedLayout(m_mainWidget);
	m_stackedLayout->setContentsMargins(0, 0, 0, 0);
	mainVLayout->addLayout(m_stackedLayout);
	connect(m_stackedLayout, &QStackedLayout::currentChanged, this, &DecryptDialog::onPageChanged);

	m_loadingPage = new LoadingPage(this);
	m_loadingPage->setMouseTracking(true);
	m_stackedLayout->addWidget(m_loadingPage);
	connect(m_loadingPage, &LoadingPage::sigLoadingFinished, this, &DecryptDialog::gotoWxProcessListPage);

	m_wxProcessListPage = new WxProcessListPage(this);
	m_wxProcessListPage->setMouseTracking(true);
	m_stackedLayout->addWidget(m_wxProcessListPage);
	connect(m_wxProcessListPage, &WxProcessListPage::sigRefresh, this, &DecryptDialog::gotoLoadingPage);
	connect(m_wxProcessListPage, &WxProcessListPage::sigStartDecrypt, this, &DecryptDialog::gotoDecryptingPage);
	connect(m_wxProcessListPage, &WxProcessListPage::sigReuseLastResultBeginMsgView, this, &DecryptDialog::sigBeginMsgView);

	m_decryptingPage = new DecryptingPage(this);
	m_decryptingPage->setMouseTracking(true);
	m_stackedLayout->addWidget(m_decryptingPage);
	connect(m_decryptingPage, &DecryptingPage::sigReDecrypt, this, &DecryptDialog::gotoDecryptingPage);
	connect(m_decryptingPage, &DecryptingPage::sigBeginMsgView, this, &DecryptDialog::sigBeginMsgView);

	QFile qssFile(":/stylesheet/decryptdlg.qss");
	if (qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		setStyleSheet(qssFile.readAll());
	}
	qssFile.close();
}

void DecryptDialog::gotoLoadingPage()
{
	if (m_stackedLayout->indexOf(m_loadingPage) == -1)
		m_stackedLayout->addWidget(m_loadingPage);
	m_stackedLayout->setCurrentWidget(m_loadingPage);
	m_loadingPage->startWork();
}

void DecryptDialog::gotoWxProcessListPage()
{
	if (m_stackedLayout->indexOf(m_wxProcessListPage) == -1)
		m_stackedLayout->addWidget(m_wxProcessListPage);
	m_stackedLayout->setCurrentWidget(m_wxProcessListPage);
	m_wxProcessListPage->startWork();
}

void DecryptDialog::gotoDecryptingPage()
{
	if (m_stackedLayout->indexOf(m_decryptingPage) == -1)
		m_stackedLayout->addWidget(m_decryptingPage);
	m_stackedLayout->setCurrentWidget(m_decryptingPage);
	m_decryptingPage->startWork();
}

void DecryptDialog::onPageChanged(int index)
{
	if (index == m_stackedLayout->indexOf(m_loadingPage))
		setFixedSize(DPI_SIZE(320, 252));
	else if (index == m_stackedLayout->indexOf(m_wxProcessListPage))
		setFixedSize(DPI_SIZE(500, 336));
	else if (index == m_stackedLayout->indexOf(m_decryptingPage))
		setFixedSize(DPI_SIZE(500, 352));
}
