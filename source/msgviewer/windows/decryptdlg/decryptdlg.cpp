#include "stdafx.h"

#include "decryptdlg.h"

#include <QStackedLayout>
#include <QFile>
#include <QPixmap>

#include "widgets/loadingpage.h"
#include "widgets/wxprocesslistpage.h"
#include "widgets/decryptingpage.h"

#include "junuoui/customwidget/junuobasetitlebar.h"

struct DecryptDialog::Data
{
	void initUI()
	{
		mainWidget = new QWidget(q);
		mainWidget->setObjectName("mainWidget");
		QVBoxLayout* vLayout = new QVBoxLayout(q);
		vLayout->setContentsMargins(DPI(10), DPI(10), DPI(10), DPI(10));
		titleBar = new JunuoBaseTitleBar(QPixmap(":/icon_svg/wxchat.svg"), q->tr("wechat db decrypt"), q);
		titleBar->setObjectName("titleBar");
		titleBar->setTitleIconSize(DPI_SIZE(18, 18));
		q->setTitleBar(titleBar);
		vLayout->addWidget(mainWidget);
		QVBoxLayout* mainVLayout = new QVBoxLayout(mainWidget);
		mainVLayout->setContentsMargins(0, 0, 0, 0);
		mainVLayout->setSpacing(0);
		mainVLayout->addWidget(titleBar);
		stackedLayout = new QStackedLayout(mainWidget);
		stackedLayout->setContentsMargins(0, 0, 0, 0);
		mainVLayout->addLayout(stackedLayout);
		DecryptDialog::connect(stackedLayout, &QStackedLayout::currentChanged, q, &DecryptDialog::onPageChanged);
		loadingPage = new LoadingPage(q);
		loadingPage->setMouseTracking(true);
		stackedLayout->addWidget(loadingPage);
		wxProcessListPage = new WxProcessListPage(q);
		wxProcessListPage->setMouseTracking(true);
		stackedLayout->addWidget(wxProcessListPage);
		DecryptDialog::connect(wxProcessListPage, &WxProcessListPage::sigRefresh, q, &DecryptDialog::sigRefresh);
		DecryptDialog::connect(wxProcessListPage, &WxProcessListPage::sigStartDecrypt, q, &DecryptDialog::sigStartDecrypt);
		DecryptDialog::connect(wxProcessListPage, &WxProcessListPage::sigReuseLastResultBeginMsgView, q, &DecryptDialog::sigBeginMsgView);
	
		decryptingPage = new DecryptingPage(q);
		decryptingPage->setMouseTracking(true);
		stackedLayout->addWidget(decryptingPage);
		DecryptDialog::connect(decryptingPage, &DecryptingPage::sigReDecrypt, q, &DecryptDialog::sigStartDecrypt);
		DecryptDialog::connect(decryptingPage, &DecryptingPage::sigBeginMsgView, q, &DecryptDialog::sigBeginMsgView);
		
		QFile qssFile(":/stylesheet/decryptdlg.qss");
		if (qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			q->setStyleSheet(qssFile.readAll());
		}
		qssFile.close();
	}

	QWidget* mainWidget = nullptr;
	DecryptDialog* q = nullptr;
	LoadingPage* loadingPage = nullptr;
	WxProcessListPage* wxProcessListPage = nullptr;
	DecryptingPage* decryptingPage = nullptr;
	QStackedLayout* stackedLayout = nullptr;
	JunuoBaseTitleBar* titleBar = nullptr;
};

DecryptDialog::DecryptDialog(QWidget* parent)
	: JunuoFrameLessWidget(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
	setMainWidget(data->mainWidget);
	setWindowIcon(QIcon(":/icon_svg/wxchat.svg"));
}

DecryptDialog::~DecryptDialog()
{

}

void DecryptDialog::gotoLoadingPage()
{
	if (data->stackedLayout->indexOf(data->loadingPage) == -1)
		data->stackedLayout->addWidget(data->loadingPage);
	data->stackedLayout->setCurrentWidget(data->loadingPage);
	data->loadingPage->startLoadingMovie();
}

void DecryptDialog::gotoWxProcessListPage()
{
	if (data->stackedLayout->indexOf(data->wxProcessListPage) == -1)
		data->stackedLayout->addWidget(data->wxProcessListPage);
	data->stackedLayout->setCurrentWidget(data->wxProcessListPage);
	data->wxProcessListPage->resetContent();
}

void DecryptDialog::gotoDecryptingPage()
{
	if (data->stackedLayout->indexOf(data->decryptingPage) == -1)
		data->stackedLayout->addWidget(data->decryptingPage);
	data->stackedLayout->setCurrentWidget(data->decryptingPage);
}

void DecryptDialog::onBeginDecrypt(size_t totalCount)
{
	data->decryptingPage->onBeginDecrypt(totalCount);
	gotoDecryptingPage();
}

void DecryptDialog::onDecryptDoneOneFile(bool isSuccess)
{
	data->decryptingPage->onDecryptDoneOneFile(isSuccess);
}

void DecryptDialog::onDecryptFinished()
{
	data->decryptingPage->onDecryptFinished();
}

void DecryptDialog::onCombineFinished(bool isSuccess)
{
	data->decryptingPage->onCombineFinished(isSuccess);
}

void DecryptDialog::onCombineOneFinished(bool isSuccess)
{
	data->decryptingPage->onCombineOneFinished(isSuccess);
}

void DecryptDialog::onCombineStarted(int totalCount)
{
	data->decryptingPage->onCombineStarted(totalCount);
}

void DecryptDialog::onPageChanged(int index)
{
	if (data->loadingPage && index != data->stackedLayout->indexOf(data->loadingPage))
		data->loadingPage->stopLoadingMovie();
	if (index == data->stackedLayout->indexOf(data->loadingPage))
		setFixedSize(DPI_SIZE(320, 252));
	else if (index == data->stackedLayout->indexOf(data->wxProcessListPage))
		setFixedSize(DPI_SIZE(500, 336));
	else if (index == data->stackedLayout->indexOf(data->decryptingPage))
		setFixedSize(DPI_SIZE(500, 352));
}

