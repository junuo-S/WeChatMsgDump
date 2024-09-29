#include "stdafx.h"

#include "decryptingpage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

#include "junuoui/progressbar/junuowaterprogressbar.h"
#include "junuoui/button/buttons.h"
#include "msgcore/glue/decryptorwapper.h"

struct DecryptingPage::Data
{
	void initUI()
	{
		mainVLayout = new QVBoxLayout(q);
		mainVLayout->setContentsMargins(0, 0, 0, DPI(4));
		progressBar = new JunuoWaterProgressBar(q);
		mainVLayout->addWidget(progressBar);
		tipLabel = new QLabel(q);
		tipLabel->setAlignment(Qt::AlignCenter);
		mainVLayout->addWidget(tipLabel);
		buttonHLayout = new QHBoxLayout(q);
		{
			beginViewButton = new JunuoBaseButton(q);
			beginViewButton->setBgColor(0x67c23a, 0x85ce61, 0x5daf34, 0xb3e19d);
			beginViewButton->setTextColor(Qt::white);
			beginViewButton->setText(DecryptingPage::tr("begin view msg"));
			DecryptingPage::connect(beginViewButton, &JunuoBaseButton::clicked, q, &DecryptingPage::sigBeginMsgView);
			reDecryptButton = new JunuoBaseButton(q);
			reDecryptButton->setBgColor(Qt::white, 0xF5F6F7, 0xEBEEF2);
			reDecryptButton->setTextColor(0x434B5A);
			reDecryptButton->setHasBorder(true);
			reDecryptButton->setBorderColor(0x666666);
			reDecryptButton->setText(DecryptingPage::tr("re decrypt"));
			DecryptingPage::connect(reDecryptButton, &JunuoBaseButton::clicked, q, &DecryptingPage::sigReDecrypt);
			buttonHLayout->addWidget(beginViewButton);
			buttonHLayout->addWidget(reDecryptButton);
			buttonHLayout->setAlignment(Qt::AlignCenter);
		}
		mainVLayout->addLayout(buttonHLayout);
		mainVLayout->setStretch(0, 5);
		mainVLayout->setStretch(1, 1);
		mainVLayout->setStretch(2, 1);
	}

	void initStyle()
	{
		progressBar->setWaterColor(0x00d200);
		progressBar->setRadius(DPI(80));
	}

	DecryptingPage* q = nullptr;
	JunuoWaterProgressBar* progressBar = nullptr;
	QVBoxLayout* mainVLayout = nullptr;
	QLabel* tipLabel = nullptr;
	QHBoxLayout* buttonHLayout = nullptr;
	JunuoBaseButton* beginViewButton = nullptr;
	JunuoBaseButton* reDecryptButton = nullptr;
	DecryptorWapper* wapper = nullptr;
};

DecryptingPage::DecryptingPage(DecryptorWapper* wapper, QWidget* parent)
	: QWidget(parent)
	, data(new Data)
{
	data->wapper = wapper;
	data->q = this;
	data->initUI();
	data->initStyle();
	connect(wapper, &DecryptorWapper::sigUpdateProgress, this, &DecryptingPage::onUpdateProgress);
	connect(wapper, &DecryptorWapper::sigDecryptFailed, this, &DecryptingPage::onDecryptFailed);
	connect(wapper, &DecryptorWapper::sigDecryptFinished, this, &DecryptingPage::onDecryptFinished);
	connect(wapper, &DecryptorWapper::sigCombineFailed, this, &DecryptingPage::onCombineFailed);
	connect(wapper, &DecryptorWapper::sigCombineFinished, this, &DecryptingPage::onCombineFinished);
}

DecryptingPage::~DecryptingPage()
{

}

void DecryptingPage::startWork()
{
	disableButtons();
	data->tipLabel->setText(tr("decrypting..."));
	data->wapper->decryptAndCombine();
	data->progressBar->startTimer();
}

void DecryptingPage::onUpdateProgress(int current, int total)
{
	data->progressBar->setMaxmumValue(total);
	data->progressBar->setValue(current);
}

void DecryptingPage::onDecryptFailed()
{
	data->tipLabel->setText(tr("decrypt failed"));
	data->progressBar->setMaxmumValue(0);
	data->progressBar->setValue(0);
	data->progressBar->stopTimer();
}

void DecryptingPage::onDecryptFinished()
{
	data->tipLabel->setText(tr("decrypt success"));
	QTimer::singleShot(100, this, [this]() { data->tipLabel->setText(tr("merging...")); });
}

void DecryptingPage::onCombineFailed()
{
	data->tipLabel->setText(tr("merge failed"));
	data->progressBar->setMaxmumValue(0);
	data->progressBar->setValue(0);
	data->progressBar->stopTimer();
}

void DecryptingPage::onCombineFinished()
{
	enableButtons();
	data->tipLabel->setText(tr("merge success"));
	data->progressBar->setValue(data->progressBar->getMaxmumValue());
	data->progressBar->stopTimer();
}

void DecryptingPage::disableButtons()
{
	data->beginViewButton->setDisabled(true);
	data->reDecryptButton->setDisabled(true);
}

void DecryptingPage::enableButtons()
{
	data->beginViewButton->setDisabled(false);
	data->reDecryptButton->setDisabled(false);
}

void DecryptingPage::showEvent(QShowEvent* event)
{
	data->beginViewButton->adjustBestSize();
	data->reDecryptButton->adjustBestSize();
}

