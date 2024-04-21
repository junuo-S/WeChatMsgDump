#include "stdafx.h"

#include "wxprocesslistpage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "dbdecryptor/wxmemoryreader/wxmemoryreader.h"

struct WxProcessListPage::Data
{
	void initUI()
	{
		mainLayout = new QVBoxLayout(q);

		tipHLayout = new QHBoxLayout(q);
		{
			tipLabel = new QLabel(q);
			refreshLabel = new QLabel(q);
			tipHLayout->addWidget(tipLabel);
			tipHLayout->addWidget(refreshLabel);
		}
		mainLayout->addLayout(tipHLayout);
		mainLayout->addSpacing(DPI(12));

		processInfoHLayout = new QHBoxLayout(q);
		{
			processHintLabel = new QLabel(WxProcessListPage::tr("processs id: "), q);
			processLabel = new QLabel(q);
			processInfoHLayout->addWidget(processHintLabel);
			processInfoHLayout->addWidget(processLabel);
		}
		mainLayout->addLayout(processInfoHLayout);

		userNameHLayout = new QHBoxLayout(q);
		{
			userNameHintLabel = new QLabel(WxProcessListPage::tr("username: "), q);
			userNameLabel = new QLabel(q);
			userNameHLayout->addWidget(userNameHintLabel);
			userNameHLayout->addWidget(userNameLabel);
		}
		mainLayout->addLayout(userNameHLayout);

		wxNumberHLayout = new QHBoxLayout(q);
		{
			wxNumberHintLabel = new QLabel(WxProcessListPage::tr("wxnumber: "), q);
			wxNumberLabel = new QLabel(q);
			wxNumberHLayout->addWidget(wxNumberHintLabel);
			wxNumberHLayout->addWidget(wxNumberLabel);
		}
		mainLayout->addLayout(wxNumberHLayout);

		phoneNumberHLayout = new QHBoxLayout(q);
		{
			phoneNumberHintLabel = new QLabel(WxProcessListPage::tr("phone number: "), q);
			phoneNumberLabel = new QLabel(q);
			phoneNumberHLayout->addWidget(phoneNumberHintLabel);
			phoneNumberHLayout->addWidget(phoneNumberLabel);
		}
		mainLayout->addLayout(phoneNumberHLayout);
	}

	void initStyle()
	{
		
	}

	void resetContent()
	{
		auto reader = WxMemoryReader::instance();
		if (reader->isSuccessFulRead())
		{
			processLabel->setText(QString::number(reader->getWxProcessId()));
			userNameLabel->setText(QString::fromStdWString(reader->getWxUserName()));
			wxNumberLabel->setText(QString::fromStdString(reader->getWxNumber()));
			phoneNumberLabel->setText(QString::fromStdString(reader->getWxPhoneNumber()));
		}
		else
		{
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
}

WxProcessListPage::~WxProcessListPage()
{

}

void WxProcessListPage::resetContent()
{
	data->resetContent();
}

