#include "stdafx.h"

#include "loadingpage.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QPainter>

#include <junuoui/customwidget/junuoloadingwidget.h>

#include "msgapplication.h"

struct LoadingPage::Data
{
	void initUI()
	{
		mainLayout = new QVBoxLayout(q);
		mainLayout->setContentsMargins(0, 0, 0, 0);
		mainLayout->setAlignment(Qt::AlignHCenter);
		titleLabel = new QLabel(q);
		titleLabel->setObjectName("titleLabel");
		titleLabel->setText(LoadingPage::tr("welcome to wechat msg dump"));
		m_loadingWidget = new JunuoNormalLoadingWidget(q);
		loadingTextLabel = new QLabel(q);
		loadingTextLabel->setObjectName("loadingTextLabel");
		loadingTextLabel->setText(QString("<center>%1</center><center>%2</center>").arg(LoadingPage::tr("detecting wechat process")).arg(LoadingPage::tr("please wait")));
		mainLayout->addWidget(titleLabel);
		mainLayout->addSpacing(DPI(16));
		mainLayout->addWidget(m_loadingWidget);
		mainLayout->addWidget(loadingTextLabel);
	}

	LoadingPage* q = nullptr;
	QVBoxLayout* mainLayout = nullptr;
	QLabel* titleLabel = nullptr;
	JunuoNormalLoadingWidget* m_loadingWidget = nullptr;
	QLabel* loadingTextLabel = nullptr;
};

LoadingPage::LoadingPage(QWidget* parent)
	: QWidget(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
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
