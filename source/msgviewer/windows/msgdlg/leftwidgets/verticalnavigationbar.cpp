#include "stdafx.h"

#include "verticalnavigationbar.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMouseEvent>

struct VerticalNavigationBar::Data
{
	void initUI()
	{
		q->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
		q->setFixedWidth(DPI(56));
		q->setAutoFillBackground(true);
		mainVLayout = new QVBoxLayout(q);
		mainVLayout->setAlignment(Qt::AlignHCenter);
		mainVLayout->setContentsMargins(DPI(13), DPI(35), DPI(13), 0);
		headImageButton = new QPushButton(q);
		headImageButton->setObjectName("headImageButton");
		headImageButton->setFixedSize(DPI(30), DPI(30));
		headImageButton->setIcon(QIcon(":/icon_svg/head-image-none.svg"));
		headImageButton->setIconSize(QSize(DPI(30), DPI(30)));
		mainVLayout->addWidget(headImageButton);
		mainVLayout->addSpacing(DPI(20));

		msgRadioButton = new QRadioButton(q);
		msgRadioButton->setObjectName("msgRadioButton");
		mainVLayout->addWidget(msgRadioButton, 0, Qt::AlignHCenter);
		mainVLayout->addSpacing(DPI(12));

		friendRadioButton = new QRadioButton(q);
		friendRadioButton->setObjectName("friendRadioButton");
		mainVLayout->addWidget(friendRadioButton);

		mainVLayout->addStretch();
	}

	VerticalNavigationBar* q = nullptr;
	QVBoxLayout* mainVLayout = nullptr;
	QPushButton* headImageButton = nullptr;
	QRadioButton* msgRadioButton = nullptr;
	QRadioButton* friendRadioButton = nullptr;
	QPoint lastPos;
	bool bLeftButtonDown = false;
};

VerticalNavigationBar::VerticalNavigationBar(QWidget* parent /*= nullptr*/)
	: QFrame(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
}

VerticalNavigationBar::~VerticalNavigationBar()
{

}

void VerticalNavigationBar::mousePressEvent(QMouseEvent* event)
{
	if (event && event->button() == Qt::LeftButton)
	{
		data->bLeftButtonDown = true;
		data->lastPos = event->pos();
	}
}

void VerticalNavigationBar::mouseReleaseEvent(QMouseEvent* event)
{
	if (event && event->button() == Qt::LeftButton)
		data->bLeftButtonDown = false;
}

void VerticalNavigationBar::mouseMoveEvent(QMouseEvent* event)
{
	QPoint delta = event->pos() - data->lastPos;
	QWidget* dialog = parent() && parent()->parent() ? qobject_cast<QWidget*>(parent()->parent()) : nullptr;
	if (dialog)
		dialog->move(dialog->pos() + delta);
}
