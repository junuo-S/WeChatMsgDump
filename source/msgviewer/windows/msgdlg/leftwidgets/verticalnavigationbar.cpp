#include "stdafx.h"

#include "verticalnavigationbar.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMouseEvent>
#include <QButtonGroup>

constexpr static const char* const gs_strIndex = "index";

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
		headImageButton->setFixedSize(HEAD_IMAGE_ICON_SIZE);
		headImageButton->setIcon(QIcon(":/icon_svg/head-image-none.svg"));
		headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
		headImageButton->setCursor(Qt::PointingHandCursor);
		mainVLayout->addWidget(headImageButton);
		mainVLayout->addSpacing(DPI(20));

		VerticalNavigationBar::connect(&radioButtonGroup, &QButtonGroup::buttonClicked, q, &VerticalNavigationBar::onRadioButtonClicked);
		msgRadioButton = new QRadioButton(q);
		msgRadioButton->setObjectName("msgRadioButton");
		msgRadioButton->setCursor(Qt::PointingHandCursor);
		msgRadioButton->setProperty(gs_strIndex, 0);
		radioButtonGroup.addButton(msgRadioButton, 0);
		mainVLayout->addWidget(msgRadioButton, 0, Qt::AlignHCenter);
		mainVLayout->addSpacing(DPI(12));

		friendRadioButton = new QRadioButton(q);
		friendRadioButton->setObjectName("friendRadioButton");
		friendRadioButton->setCursor(Qt::PointingHandCursor);
		friendRadioButton->setProperty(gs_strIndex, 1);
		radioButtonGroup.addButton(friendRadioButton, 1);
		mainVLayout->addWidget(friendRadioButton);

		mainVLayout->addStretch();
	}

	VerticalNavigationBar* q = nullptr;
	QVBoxLayout* mainVLayout = nullptr;
	QPushButton* headImageButton = nullptr;
	QRadioButton* msgRadioButton = nullptr;
	QRadioButton* friendRadioButton = nullptr;
	QButtonGroup radioButtonGroup;
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

void VerticalNavigationBar::setHeadImage(const QIcon& icon)
{
	data->headImageButton->setIcon(icon);
}

void VerticalNavigationBar::setCurrentPage(unsigned int index)
{
	if (auto button = data->radioButtonGroup.button(index))
		button->setChecked(true);
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

void VerticalNavigationBar::onRadioButtonClicked(QAbstractButton* button)
{
	if (button)
		emit sigCurrentPageChanged(button->property(gs_strIndex).toUInt());
}
