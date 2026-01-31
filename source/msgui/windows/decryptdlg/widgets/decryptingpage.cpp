#include "stdafx.h"

#include "decryptingpage.h"
#include <QTimer>

#include "junuoui/progressbar/junuowaterprogressbar.h"
#include "junuoui/button/buttons.h"

#include "msgapplication.h"

DecryptingPage::DecryptingPage(QWidget* parent)
	: QWidget(parent)
{
	initUI();
	initStyle();
	if (ComPtr<IJCoreApplication> coreApp = msgApp->GetCoreApplication())
		attachTo(coreApp->GetDecryptManager());
}

DecryptingPage::~DecryptingPage()
{

}

void DecryptingPage::startWork()
{
	m_beginViewButton->hide();
	m_reDecryptButton->hide();
	m_tipLabel->setText(tr("decrypting..."));
	m_progressBar->startTimer();
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
	m_progressBar->setValue(current);
}

void DecryptingPage::onDecryptFinished()
{
	m_tipLabel->setText(tr("decrypt success"));
	QTimer::singleShot(100, this, [this]() { m_tipLabel->setText(tr("merging...")); });
}

void DecryptingPage::onDecryptFailed()
{
	m_tipLabel->setText(tr("decrypt failed"));
	m_progressBar->setValue(0);
	m_progressBar->stopTimer();
	m_reDecryptButton->show();
}

void DecryptingPage::onCombineFinished()
{
	m_tipLabel->setText(tr("merge success"));
	m_progressBar->setValue(m_progressBar->getMaxmumValue());
	m_progressBar->stopTimer();
	m_beginViewButton->show();
}

void DecryptingPage::showEvent(QShowEvent* event)
{
	m_beginViewButton->adjustBestSize();
	m_reDecryptButton->adjustBestSize();
}

void DecryptingPage::initUI()
{
	m_mainVLayout = new QVBoxLayout(this);
	m_mainVLayout->setContentsMargins(0, 0, 0, DPI(4));

	m_progressBar = new JunuoWaterProgressBar(this);
	m_progressBar->setMaxmumValue(100);
	m_mainVLayout->addWidget(m_progressBar);

	m_tipLabel = new QLabel(this);
	m_tipLabel->setAlignment(Qt::AlignCenter);
	m_mainVLayout->addWidget(m_tipLabel);

	m_buttonHLayout = new QHBoxLayout(this);
	{
		m_beginViewButton = new JunuoBaseButton(this);
		m_beginViewButton->setBgColor(0x67c23a, 0x85ce61, 0x5daf34, 0xb3e19d);
		m_beginViewButton->setTextColor(Qt::white);
		m_beginViewButton->setText(DecryptingPage::tr("begin view msg"));
		connect(m_beginViewButton, &JunuoBaseButton::clicked, this, &DecryptingPage::sigBeginMsgView);

		m_reDecryptButton = new JunuoBaseButton(this);
		m_reDecryptButton->setBgColor(Qt::white, 0xF5F6F7, 0xEBEEF2);
		m_reDecryptButton->setTextColor(0x434B5A);
		m_reDecryptButton->setHasBorder(true);
		m_reDecryptButton->setBorderColor(0x666666);
		m_reDecryptButton->setText(DecryptingPage::tr("re decrypt"));
		connect(m_reDecryptButton, &JunuoBaseButton::clicked, this, &DecryptingPage::sigReDecrypt);

		m_buttonHLayout->addWidget(m_beginViewButton);
		m_buttonHLayout->addWidget(m_reDecryptButton);
		m_buttonHLayout->setAlignment(Qt::AlignCenter);
	}
	m_mainVLayout->addLayout(m_buttonHLayout);
	m_mainVLayout->setStretch(0, 5);
	m_mainVLayout->setStretch(1, 1);
	m_mainVLayout->setStretch(2, 1);
}

void DecryptingPage::initStyle()
{
	m_progressBar->setWaterColor(0x00d200);
	m_progressBar->setRadius(DPI(80));
}

