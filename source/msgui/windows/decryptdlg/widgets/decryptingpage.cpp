#include "stdafx.h"

#include "decryptingpage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

#include "junuoui/progressbar/junuowaterprogressbar.h"
#include "junuoui/button/buttons.h"

#include "msgapplication.h"

struct DecryptingPage::Data
{
	void initUI()
	{
		mainVLayout = new QVBoxLayout(q);
		mainVLayout->setContentsMargins(0, 0, 0, DPI(4));
		progressBar = new JunuoWaterProgressBar(q);
		progressBar->setMaxmumValue(100);
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
};

DecryptingPage::DecryptingPage(QWidget* parent)
	: QWidget(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
	data->initStyle();
	if (ComPtr<IJCoreApplication> coreApp = msgApp->GetCoreApplication())
		attachTo(coreApp->GetDecryptManager());
}

DecryptingPage::~DecryptingPage()
{

}

void DecryptingPage::startWork()
{
	data->beginViewButton->hide();
	data->reDecryptButton->hide();
	data->tipLabel->setText(tr("decrypting..."));
	data->progressBar->startTimer();
	ComPtr<IJCoreApplication> coreApp = msgApp->GetCoreApplication();
	ComPtr<IJWeChatDBDecryptManager> decryptManager = coreApp ? coreApp->GetDecryptManager() : nullptr;
	if (!decryptManager || !decryptManager->StartDecryptDataBase())
		onDecryptFailed();
}

STDMETHODIMP_(bool) DecryptingPage::OnCoreEvent(IJCoreEvent* event)
{
	if (event->Type() == EventType::Event_Decrypt || event->Type() == EventType::Event_Combine)
	{
		JCommonAsyncEvent* asyncEvent = dynamic_cast<JCommonAsyncEvent*>(event);
		QMetaObject::invokeMethod(this, "processCoreEvent", Qt::QueuedConnection, Q_ARG(JCommonAsyncEvent, *asyncEvent));
		return true;
	}
	return false;
}

Q_INVOKABLE void DecryptingPage::processCoreEvent(const JCommonAsyncEvent& event)
{
	if (event.m_subType == JCommonAsyncEvent::SubType::SubType_Progress)
	{
		updateProgress(event.m_progress * 100);
	}
	else if (event.m_subType == JCommonAsyncEvent::SubType::SubType_End)
	{
		if (event.m_type == EventType::Event_Decrypt)
			onDecryptFinished();
		else if (event.m_type == EventType::Event_Combine)
			onCombineFinished();
	}
}

void DecryptingPage::updateProgress(int current)
{
	data->progressBar->setValue(current);
}

void DecryptingPage::onDecryptFinished()
{
	data->tipLabel->setText(tr("decrypt success"));
	QTimer::singleShot(100, this, [this]() { data->tipLabel->setText(tr("merging...")); });
}

void DecryptingPage::onDecryptFailed()
{
	data->tipLabel->setText(tr("decrypt failed"));
	data->progressBar->setValue(0);
	data->progressBar->stopTimer();
	data->reDecryptButton->show();
}

void DecryptingPage::onCombineFinished()
{
	data->tipLabel->setText(tr("merge success"));
	data->progressBar->setValue(data->progressBar->getMaxmumValue());
	data->progressBar->stopTimer();
	data->beginViewButton->show();
}

void DecryptingPage::showEvent(QShowEvent* event)
{
	data->beginViewButton->adjustBestSize();
	data->reDecryptButton->adjustBestSize();
}

