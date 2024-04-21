#include "stdafx.h"

#include "wxprocesslistpage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

#include "dbdecryptor/wxmemoryreader/wxmemoryreader.h"

struct WxProcessListPage::Data
{
	void initUI()
	{
		mainLayout = new QVBoxLayout(q);
		mainLayout->setContentsMargins(20, 20, 20, 8);

		tipHLayout = new QHBoxLayout(q);
		{
			tipLabel = new QLabel(q);
			tipLabel->setObjectName("title");
			tipLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
			refreshLabel = new QLabel(q);
			refreshLabel->setToolTip(WxProcessListPage::tr("refresh"));
			tipHLayout->addWidget(tipLabel);
			tipHLayout->addWidget(refreshLabel);
		}
		tipHLayout->setAlignment(Qt::AlignCenter);
		mainLayout->addLayout(tipHLayout);
		mainLayout->addSpacing(DPI(12));

		processInfoHLayout = new QHBoxLayout(q);
		{
			processHintLabel = new QLabel(WxProcessListPage::tr("processs id: "), q);
			processLabel = new QLabel(q);
			processInfoHLayout->addWidget(processHintLabel);
			processInfoHLayout->addWidget(processLabel);
		}
		processInfoHLayout->setStretch(0, 1);
		processInfoHLayout->setStretch(1, 3);
		mainLayout->addLayout(processInfoHLayout);

		userNameHLayout = new QHBoxLayout(q);
		{
			userNameHintLabel = new QLabel(WxProcessListPage::tr("username: "), q);
			userNameLabel = new QLabel(q);
			userNameHLayout->addWidget(userNameHintLabel);
			userNameHLayout->addWidget(userNameLabel);
		}
		userNameHLayout->setStretch(0, 1);
		userNameHLayout->setStretch(1, 3);
		mainLayout->addLayout(userNameHLayout);

		wxNumberHLayout = new QHBoxLayout(q);
		{
			wxNumberHintLabel = new QLabel(WxProcessListPage::tr("wxnumber: "), q);
			wxNumberLabel = new QLabel(q);
			wxNumberHLayout->addWidget(wxNumberHintLabel);
			wxNumberHLayout->addWidget(wxNumberLabel);
		}
		wxNumberHLayout->setStretch(0, 1);
		wxNumberHLayout->setStretch(1, 3);
		mainLayout->addLayout(wxNumberHLayout);

		phoneNumberHLayout = new QHBoxLayout(q);
		{
			phoneNumberHintLabel = new QLabel(WxProcessListPage::tr("phone number: "), q);
			phoneNumberLabel = new QLabel(q);
			phoneNumberHLayout->addWidget(phoneNumberHintLabel);
			phoneNumberHLayout->addWidget(phoneNumberLabel);
		}
		phoneNumberHLayout->setStretch(0, 1);
		phoneNumberHLayout->setStretch(1, 3);
		mainLayout->addLayout(phoneNumberHLayout);
		mainLayout->addStretch();
	}

	void initStyle()
	{
		refreshLabel->setPixmap(QPixmap(":/icon_svg/refresh.svg").scaled(DPI_SIZE(24, 24)));
		refreshLabel->setCursor(Qt::PointingHandCursor);
		QFont font("Microsoft YaHei");
		font.setPointSize(14);
		tipLabel->setFont(font);
		font.setPointSize(9);
		processHintLabel->setFont(font);
		processLabel->setFont(font);
		userNameHintLabel->setFont(font);
		userNameLabel->setFont(font);
		wxNumberHintLabel->setFont(font);
		wxNumberLabel->setFont(font);
		phoneNumberHintLabel->setFont(font);
		phoneNumberLabel->setFont(font);
	}

	void resetContent()
	{
		auto reader = WxMemoryReader::instance();
		if (reader->isSuccessFulRead())
		{
			tipLabel->setText(WxProcessListPage::tr("read success tip"));
			processLabel->setText(QString::number(reader->getWxProcessId()));
			userNameLabel->setText(QString::fromStdWString(reader->getWxUserName()));
			wxNumberLabel->setText(QString::fromStdString(reader->getWxNumber()));
			phoneNumberLabel->setText(QString::fromStdString(reader->getWxPhoneNumber()));
		}
		else
		{
			tipLabel->setText(WxProcessListPage::tr("read failed tip"));
			QString readFailed = WxProcessListPage::tr("read failed");
			processLabel->setText(readFailed);
			userNameLabel->setText(readFailed);
			wxNumberLabel->setText(readFailed);
			phoneNumberLabel->setText(readFailed);
		}
	}

	WxProcessListPage* q = nullptr;
	QVBoxLayout* mainLayout = nullptr;
	QHBoxLayout* tipHLayout = nullptr;
	QLabel* tipLabel = nullptr;
	QLabel* refreshLabel = nullptr;
	QHBoxLayout* processInfoHLayout = nullptr;
	QLabel* processHintLabel = nullptr;
	QLabel* processLabel = nullptr;
	QHBoxLayout* userNameHLayout = nullptr;
	QLabel* userNameHintLabel = nullptr;
	QLabel* userNameLabel = nullptr;
	QHBoxLayout* wxNumberHLayout = nullptr;
	QLabel* wxNumberHintLabel = nullptr;
	QLabel* wxNumberLabel = nullptr;
	QHBoxLayout* phoneNumberHLayout = nullptr;
	QLabel* phoneNumberHintLabel = nullptr;
	QLabel* phoneNumberLabel = nullptr;
};

WxProcessListPage::WxProcessListPage(QWidget* parent)
	: QWidget(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
	data->initStyle();
	data->refreshLabel->installEventFilter(this);
}

WxProcessListPage::~WxProcessListPage()
{

}

void WxProcessListPage::resetContent()
{
	data->resetContent();
}

bool WxProcessListPage::eventFilter(QObject* object, QEvent* event)
{
	if (object == data->refreshLabel && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent && mouseEvent->button() == Qt::LeftButton)
		{
			emit sigRefresh();
			return true;
		}
		return false;
	}
	return false;
}

