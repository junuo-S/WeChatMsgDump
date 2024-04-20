#include "stdafx.h"

#include "decryptdlg.h"

#include <QStackedLayout>

#include "widgets/loadingpage.h"

struct DecryptDialog::Data
{
	void initUI()
	{
		stackedLayout = new QStackedLayout(q);
		DecryptDialog::connect(stackedLayout, &QStackedLayout::currentChanged, q, &DecryptDialog::onPageChanged);
		loadingPage = new LoadingPage(q);
		stackedLayout->addWidget(loadingPage);
	}

	DecryptDialog* q = nullptr;
	LoadingPage* loadingPage = nullptr;
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

void DecryptDialog::onPageChanged(int index)
{
	if (data->loadingPage && index != data->stackedLayout->indexOf(data->loadingPage))
		data->loadingPage->stopLoadingMovie();
	if (index == data->stackedLayout->indexOf(data->loadingPage))
		setFixedSize(DPI_SIZE(300, 200));
}

