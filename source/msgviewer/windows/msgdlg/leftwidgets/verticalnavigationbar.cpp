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
		mainVLayout->setContentsMargins(0, DPI(30), 0, 0);
		//headImageButton = new QPushButton(q);
		//mainVLayout->addWidget(headImageButton);
		//mainVLayout->addStretch();
	}

	VerticalNavigationBar* q = nullptr;
	QVBoxLayout* mainVLayout = nullptr;
	QPushButton* headImageButton = nullptr;
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
