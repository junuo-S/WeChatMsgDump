#include "stdafx.h"

#include "wxprocesslistpage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>

#include "msgcore/databus/databus.h"
#include "msgcore/glue/decryptorwapper.h"
#include "junuoui/button/buttons.h"

struct WxProcessListPage::Data
{
	void initUI()
	{
		mainLayout = new QVBoxLayout(q);
		mainLayout->setContentsMargins(20, 0, 20, 4);

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

		wxVersionLayout = new QHBoxLayout(q);
		{
			wxVersionHintLabel = new QLabel(WxProcessListPage::tr("wechat version: "), q);
			wxVersionLabel = new QLabel(q);
			wxVersionLayout->addWidget(wxVersionHintLabel);
			wxVersionLayout->addWidget(wxVersionLabel);
		}
		wxVersionLayout->setStretch(0, 1);
		wxVersionLayout->setStretch(1, 3);
		mainLayout->addLayout(wxVersionLayout);

		wxExePathLayout = new QHBoxLayout(q);
		{
			wxExePathHintLabel = new QLabel(WxProcessListPage::tr("wechat exe: "), q);
			wxExePathLabel = new QLabel(q);
			wxExePathLayout->addWidget(wxExePathHintLabel);
			wxExePathLayout->addWidget(wxExePathLabel);
		}
		wxExePathLayout->setStretch(0, 1);
		wxExePathLayout->setStretch(1, 3);
		mainLayout->addLayout(wxExePathLayout);

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

		wxidLayout = new QHBoxLayout(q);
		{
			wxidHintLabel = new QLabel(WxProcessListPage::tr("wxid: "), q);
			wxidLabel = new QLabel(q);
			wxidLayout->addWidget(wxidHintLabel);
			wxidLayout->addWidget(wxidLabel);
		}
		wxidLayout->setStretch(0, 1);
		wxidLayout->setStretch(1, 3);
		mainLayout->addLayout(wxidLayout);

		wxDataPathLayout = new QHBoxLayout(q);
		{
			wxDataPathHintLabel = new QLabel(WxProcessListPage::tr("wechat path: "), q);
			wxDataPathLabel = new QLabel(q);
			wxDataPathLayout->addWidget(wxDataPathHintLabel);
			wxDataPathLayout->addWidget(wxDataPathLabel);
		}
		wxDataPathLayout->setStretch(0, 1);
		wxDataPathLayout->setStretch(1, 3);
		mainLayout->addLayout(wxDataPathLayout);

		buttonLayout = new QHBoxLayout(q);
		{
			beginButton = new JunuoBaseButton(WxProcessListPage::tr("begin decrypt"), q);
			beginButton->setBgColor(0x409eff, 0x66b1ff, 0x3a8ee6, 0xa0cfff);
			beginButton->setTextColor(Qt::white);
			buttonLayout->addWidget(beginButton);
			WxProcessListPage::connect(beginButton, &QPushButton::clicked, q, &WxProcessListPage::sigStartDecrypt);
			reuseLastRetButton = new JunuoBaseButton(WxProcessListPage::tr("reuse last result begin"), q);
			reuseLastRetButton->setBgColor(0x67c23a, 0x85ce61, 0x5daf34, 0xb3e19d);
			reuseLastRetButton->setTextColor(Qt::white);
			buttonLayout->addWidget(reuseLastRetButton);
			WxProcessListPage::connect(reuseLastRetButton, &QPushButton::clicked, q, &WxProcessListPage::sigReuseLastResultBeginMsgView);
		}
		mainLayout->addSpacing(DPI(10));
		mainLayout->addLayout(buttonLayout);
		mainLayout->addStretch();
	}

	void initStyle()
	{
		refreshLabel->setPixmap(QPixmap(":/icon_svg/refresh.svg").scaled(DPI_SIZE(20, 20)));
		refreshLabel->setCursor(Qt::PointingHandCursor);
	}

	void resetContent()
	{
		if (DATA_BUS_INSTANCE->getMemoryReadSuc())
		{
			tipLabel->setText(WxProcessListPage::tr("read success tip"));
			wxVersionLabel->setText(DATA_BUS_INSTANCE->getWxVersion());
			wxExePathLabel->setText(DATA_BUS_INSTANCE->getWxExePath());
			wxExePathLabel->setToolTip(wxExePathLabel->text());
			processLabel->setText(QString::number(DATA_BUS_INSTANCE->getWxProcessId()));
			userNameLabel->setText(DATA_BUS_INSTANCE->getWxUserName());
			wxNumberLabel->setText(DATA_BUS_INSTANCE->getWxNumber());
			phoneNumberLabel->setText(DATA_BUS_INSTANCE->getWxPhoneNumber());
			wxidLabel->setText(DATA_BUS_INSTANCE->getWxid());
			wxDataPathLabel->setText(DATA_BUS_INSTANCE->getWxDataPath());
			wxDataPathLabel->setToolTip(wxDataPathLabel->text());
			beginButton->setEnabled(true);
		}
		else
		{
			tipLabel->setText(WxProcessListPage::tr("read failed tip"));
			QString readFailed = WxProcessListPage::tr("read failed");
			wxVersionLabel->setText(readFailed);
			wxExePathLabel->setText(readFailed);
			wxExePathLabel->setToolTip(wxExePathLabel->text());
			processLabel->setText(readFailed);
			userNameLabel->setText(readFailed);
			wxNumberLabel->setText(readFailed);
			phoneNumberLabel->setText(readFailed);
			wxidLabel->setText(readFailed);
			wxDataPathLabel->setText(readFailed);
			beginButton->setEnabled(false);
		}
	}

	WxProcessListPage* q = nullptr;
	QVBoxLayout* mainLayout = nullptr;
	QHBoxLayout* tipHLayout = nullptr;
	QLabel* tipLabel = nullptr;
	QLabel* refreshLabel = nullptr;
	QHBoxLayout* wxVersionLayout = nullptr;
	QLabel* wxVersionHintLabel = nullptr;
	QLabel* wxVersionLabel = nullptr;
	QHBoxLayout* wxExePathLayout = nullptr;
	QLabel* wxExePathHintLabel = nullptr;
	QLabel* wxExePathLabel = nullptr;
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
	QHBoxLayout* wxidLayout = nullptr;
	QLabel* wxidHintLabel = nullptr;
	QLabel* wxidLabel = nullptr;
	QHBoxLayout* wxDataPathLayout = nullptr;
	QLabel* wxDataPathHintLabel = nullptr;
	QLabel* wxDataPathLabel = nullptr;
	QHBoxLayout* buttonLayout = nullptr;
	JunuoBaseButton* beginButton = nullptr;
	JunuoBaseButton* reuseLastRetButton = nullptr;
	DecryptorWapper* wapper = nullptr;
};

WxProcessListPage::WxProcessListPage(DecryptorWapper* wapper, QWidget* parent)
	: QWidget(parent)
	, data(new Data)
{
	data->wapper = wapper;
	data->q = this;
	data->initUI();
	data->initStyle();
	data->refreshLabel->installEventFilter(this);
}

WxProcessListPage::~WxProcessListPage()
{

}

void WxProcessListPage::startWork()
{
	resetContent();
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

void WxProcessListPage::showEvent(QShowEvent* event)
{
	data->beginButton->adjustBestSize();
	data->reuseLastRetButton->adjustBestSize();
}

