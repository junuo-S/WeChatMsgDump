#include "stdafx.h"

#include "loadingpage.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QPainter>

#include <msgapplication.h>

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
		gifLabel = new QLabel(q);
		gifLabel->setAlignment(Qt::AlignCenter);
		loadingTextLabel = new QLabel(q);
		loadingTextLabel->setObjectName("loadingTextLabel");
		loadingTextLabel->setText(QString("<center>%1</center><center>%2</center>").arg(LoadingPage::tr("detecting wechat process")).arg(LoadingPage::tr("please wait")));
		timer = new QTimer(q);
		timer->setInterval(16);
		LoadingPage::connect(timer, &QTimer::timeout, q, &LoadingPage::onTimeOut);
		mainLayout->addWidget(titleLabel);
		mainLayout->addSpacing(DPI(16));
		mainLayout->addWidget(gifLabel);
		mainLayout->addWidget(loadingTextLabel);
	}

	void nextRotation()
	{
		if (!gifLabel)
		{
			timer->stop();
			return;
		}
		const QSize size = DPI_SIZE(48, 48);
		angle = (angle + 6) % 360;
		QImage image(size, QImage::Format_ARGB32_Premultiplied);
		image.fill(Qt::transparent);
		QPainter painter(&image);
		painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.translate(size.width() / 2, size.height() / 2);
		painter.rotate(angle);
		painter.translate(-size.width() / 2, -size.height() / 2);
		painter.drawPixmap(0, 0, pixmap.scaled(size));
		painter.end();
		gifLabel->setPixmap(QPixmap::fromImage(image));
	}

	LoadingPage* q = nullptr;
	QVBoxLayout* mainLayout = nullptr;
	QLabel* titleLabel = nullptr;
	QLabel* gifLabel = nullptr;
	QLabel* loadingTextLabel = nullptr;
	QTimer* timer = nullptr;
	int angle = 0;
	QPixmap pixmap = QPixmap(":/icon_svg/loading-light.svg");
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
	startLoadingMovie();
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

void LoadingPage::startLoadingMovie()
{
	data->timer->stop();
	data->timer->start();
}

void LoadingPage::stopLoadingMovie()
{
	data->timer->stop();
	data->angle = 0;
}

void LoadingPage::onTimeOut()
{
	data->nextRotation();
}

void LoadingPage::onMemoryReadFinished()
{
	QTimer::singleShot(2000, this, [this]()
		{
			stopLoadingMovie();
			emit sigLoadingFinished();
		});
}
