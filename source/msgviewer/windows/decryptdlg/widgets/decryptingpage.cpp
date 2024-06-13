#include "stdafx.h"

#include "decryptingpage.h"

#include <QHBoxLayout>
#include <QLabel>

#include "junuoui/progressbar/junuowaterprogressbar.h"
#include "junuoui/button/buttons.h"

struct DecryptingPage::Data
{
	void initUI()
	{
		mainVLayout = new QVBoxLayout(q);
		progressBar = new JunuoWaterProgressBar(q);
		mainVLayout->addWidget(progressBar);
		tipLabel = new QLabel(q);
		tipLabel->setAlignment(Qt::AlignCenter);
		mainVLayout->addWidget(tipLabel);
		buttonHLayout = new QHBoxLayout(q);
		{
			beginViewButton = new JunuoBaseButton(q);
			beginViewButton->setBgColor(0x67c23a, 0x85ce61, 0x5daf34);
			beginViewButton->setTextColor(Qt::white);
			beginViewButton->setText(DecryptingPage::tr("begin view msg"));
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
		QString styleSheet = QString("QLabel {"
				"font-family: Microsoft YaHei;"
				"font-size: %1px;"
				"color: #8B97AF;"
			"}"
		).arg(DPI(14));
		q->setStyleSheet(styleSheet);
		QFont font("KaiTi");
		font.setPixelSize(DPI(18));
		beginViewButton->setFont(font);
		beginViewButton->adjustBestSize();
		reDecryptButton->setFont(font);
		reDecryptButton->adjustBestSize();
		progressBar->setWaterColor(0x00d200);
		progressBar->setRadius(DPI(50));
	}

	DecryptingPage* q = nullptr;
	JunuoWaterProgressBar* progressBar = nullptr;
	QVBoxLayout* mainVLayout = nullptr;
	QLabel* tipLabel = nullptr;
	QHBoxLayout* buttonHLayout = nullptr;
	JunuoBaseButton* beginViewButton = nullptr;
	JunuoBaseButton* reDecryptButton = nullptr;
};

DecryptingPage::DecryptingPage(QWidget* parent)
	: QWidget(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
	data->initStyle();
}

DecryptingPage::~DecryptingPage()
{

}

void DecryptingPage::onBeginDecrypt(int totalCount)
{
	data->beginViewButton->setDisabled(true);
	data->reDecryptButton->setDisabled(true);
	data->tipLabel->setText(tr("decrypting..."));
	data->progressBar->setMaxmumValue(totalCount);
	data->progressBar->setValue(data->progressBar->getMinimumValue());
	data->progressBar->startTimer();
}

void DecryptingPage::onDecryptDoneOneFile(bool isSuccess)
{
	data->progressBar->setValue(data->progressBar->getValue() + 1);
}

void DecryptingPage::onDecryptFinished()
{
	data->beginViewButton->setDisabled(false);
	data->reDecryptButton->setDisabled(false);
	data->tipLabel->setText(tr("decrypt success"));
	data->progressBar->setValue(data->progressBar->getMaxmumValue());
	data->progressBar->stopTimer();
}

