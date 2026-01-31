#include "stdafx.h"

#include "loadingpage.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QPainter>

#include <junuoui/customwidget/junuoloadingwidget.h>

#include "msgapplication.h"

LoadingPage::LoadingPage(QWidget* parent)
	: QWidget(parent)
{
	initUI();
	if (auto coreApplication = msgApp->GetCoreApplication(); auto manager = coreApplication ? coreApplication->GetDecryptManager() : nullptr)
		attachTo(manager);
}

LoadingPage::~LoadingPage() = default;

void LoadingPage::startWork()
{
	if (auto coreApplication = msgApp->GetCoreApplication(); auto manager = coreApplication ? coreApplication->GetDecryptManager() : nullptr)
		manager->StartReadWeChatProcess();
}

STDMETHODIMP_(bool) LoadingPage::OnCoreEvent(IJCoreEvent* event)
{
	if (event->Type() == EventType::Event_ReadProcess)
	{
		JCommonAsyncEvent* readProcessEvent = dynamic_cast<JCommonAsyncEvent*>(event);
		if (readProcessEvent && readProcessEvent->m_subType == JCommonAsyncEvent::SubType::SubType_End)
			onMemoryReadFinished();
	}
	return false;
}

void LoadingPage::onMemoryReadFinished()
{
	QTimer::singleShot(2000, this, [this]()
		{
			emit sigLoadingFinished();
		});
}

void LoadingPage::initUI()
{
	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->setContentsMargins(0, 0, 0, 0);
	m_mainLayout->setAlignment(Qt::AlignHCenter);

	m_titleLabel = new QLabel(this);
	m_titleLabel->setObjectName("titleLabel");
	m_titleLabel->setText(LoadingPage::tr("welcome to wechat msg dump"));

	m_m_loadingWidget = new JunuoNormalLoadingWidget(this);

	m_loadingTextLabel = new QLabel(this);
	m_loadingTextLabel->setObjectName("loadingTextLabel");
	m_loadingTextLabel->setText(QString("<center>%1</center><center>%2</center>")
									.arg(LoadingPage::tr("detecting wechat process"))
									.arg(LoadingPage::tr("please wait")));

	m_mainLayout->addWidget(m_titleLabel);
	m_mainLayout->addSpacing(DPI(16));
	m_mainLayout->addWidget(m_m_loadingWidget);
	m_mainLayout->addWidget(m_loadingTextLabel);
}
