#include "stdafx.h"

#include "decryptdlg.h"

#include <QStackedLayout>

#include "widgets/loadingpage.h"
#include "widgets/wxprocesslistpage.h"
#include "widgets/decryptingpage.h"

struct DecryptDialog::Data
{
	void initUI()
	{
		stackedLayout = new QStackedLayout(q);
		DecryptDialog::connect(stackedLayout, &QStackedLayout::currentChanged, q, &DecryptDialog::onPageChanged);
		loadingPage = new LoadingPage(q);
		stackedLayout->addWidget(loadingPage);
		wxProcessListPage = new WxProcessListPage(q);
		stackedLayout->addWidget(wxProcessListPage);
		DecryptDialog::connect(wxProcessListPage, &WxProcessListPage::sigRefresh, q, &DecryptDialog::sigRefresh);
		DecryptDialog::connect(wxProcessListPage, &WxProcessListPage::sigStartDecrypt, q, &DecryptDialog::sigStartDecrypt);
		DecryptDialog::connect(wxProcessListPage, &WxProcessListPage::sigReuseLastResultBeginMsgView, q, &DecryptDialog::sigBeginMsgView);
	
		decryptingPage = new DecryptingPage(q);
		stackedLayout->addWidget(decryptingPage);
		DecryptDialog::connect(decryptingPage, &DecryptingPage::sigReDecrypt, q, &DecryptDialog::sigStartDecrypt);
		DecryptDialog::connect(decryptingPage, &DecryptingPage::sigBeginMsgView, q, &DecryptDialog::sigBeginMsgView);
	}

	DecryptDialog* q = nullptr;
	LoadingPage* loadingPage = nullptr;
	WxProcessListPage* wxProcessListPage = nullptr;
	DecryptingPage* decryptingPage = nullptr;
	QStackedLayout* stackedLayout = nullptr;
};

DecryptDialog::DecryptDialog(QWidget* parent)
	: QWidget(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
	setWindowTitle(tr("wechat db decrypt"));
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
		setFixedSize(DPI_SIZE(300, 200));
	if (index == data->stackedLayout->indexOf(data->wxProcessListPage))
		setFixedSize(DPI_SIZE(480, 300));
}

