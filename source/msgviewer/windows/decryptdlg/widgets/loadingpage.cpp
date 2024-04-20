#include "stdafx.h"

#include "loadingpage.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QPainter>

struct LoadingPage::Data
{
	void initUI()
	{
		mainLayout = new QVBoxLayout(q);
		mainLayout->setAlignment(Qt::AlignHCenter);
		titleLabel = new QLabel(q);
		titleLabel->setText(LoadingPage::tr("welcome to wechat msg dump"));
		gifLabel = new QLabel(q);
		gifLabel->setAlignment(Qt::AlignCenter);
		loadingTextLabel = new QLabel(q);
		loadingTextLabel->setText(QString("<center>%1</center><center>%2</center>").arg(LoadingPage::tr("detecting wechat process")).arg(LoadingPage::tr("please wait")));
		timer = new QTimer(q);
		timer->setInterval(16);
		LoadingPage::connect(timer, &QTimer::timeout, q, &LoadingPage::onTimeOut);
		mainLayout->addWidget(titleLabel);
		mainLayout->addSpacing(DPI(16));
		mainLayout->addWidget(gifLabel);
		mainLayout->addWidget(loadingTextLabel);
	}

	void initStyle()
	{
		QFont font("Microsoft YaHei");
		font.setPointSize(16);
		titleLabel->setFont(font);
		font.setPointSize(10);
		loadingTextLabel->setFont(font);
		auto palette = loadingTextLabel->palette();
		palette.setColor(QPalette::WindowText, QColor(QRgb(0x8B97AF)));
		loadingTextLabel->setPalette(palette);
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
	data->initStyle();
}

LoadingPage::~LoadingPage()
{

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
