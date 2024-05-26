#include "stdafx.h"

#include "wxprocesslistpage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QComboBox>

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

		wxidsLayout = new QHBoxLayout(q);
		{
			wxidsHintLabel = new QLabel(WxProcessListPage::tr("wxid: "), q);
			wxidsComboBox = new QComboBox(q);
			wxidsLayout->addWidget(wxidsHintLabel);
			wxidsLayout->addWidget(wxidsComboBox);
		}
		wxidsLayout->setStretch(0, 1);
		wxidsLayout->setStretch(1, 3);
		mainLayout->addLayout(wxidsLayout);

		wxDataPathsLayout = new QHBoxLayout(q);
		{
			wxDataPathsHintLabel = new QLabel(WxProcessListPage::tr("wechat path: "), q);
			wxDataPathsComboBox = new QComboBox(q);
			wxDataPathsLayout->addWidget(wxDataPathsHintLabel);
			wxDataPathsLayout->addWidget(wxDataPathsComboBox);
		}
		wxDataPathsLayout->setStretch(0, 1);
		wxDataPathsLayout->setStretch(1, 3);
		mainLayout->addLayout(wxDataPathsLayout);

		mainLayout->addStretch();
	}

	void initStyle()
	{
		refreshLabel->setPixmap(QPixmap(":/icon_svg/refresh.svg").scaled(DPI_SIZE(20, 20)));
		refreshLabel->setCursor(Qt::PointingHandCursor);
		QString styleSheet = QString("QLabel, QComboBox {"
			"font-family: Microsoft YaHei;"
			"font-size: %1px;"
			"}"
			"QLabel#title {"
			"font-size: %2px;"
			"}").arg(DPI(12)).arg(DPI(16));
		q->setStyleSheet(styleSheet);
	}

	void resetContent()
	{
		auto reader = WxMemoryReader::instance();
		if (reader->isSuccessFulRead())
		{
			tipLabel->setText(WxProcessListPage::tr("read success tip"));
			wxVersionLabel->setText(QString::fromStdString(reader->getWxVersion()));
			wxExePathLabel->setText(QString::fromStdWString(reader->getWxExePath()));
			wxExePathLabel->setToolTip(wxExePathLabel->text());
			processLabel->setText(QString::number(reader->getWxProcessId()));
			userNameLabel->setText(QString::fromStdWString(reader->getWxUserName()));
			wxNumberLabel->setText(QString::fromStdString(reader->getWxNumber()));
			phoneNumberLabel->setText(QString::fromStdString(reader->getWxPhoneNumber()));
			auto wxids = reader->getWxids();
			for (auto cit = wxids.cbegin(); cit != wxids.cend(); cit++)
				wxidsComboBox->addItem(QString::fromStdString(*cit));
			auto wxDataPaths = reader->getWxDataPaths();
			for (auto cit = wxDataPaths.cbegin(); cit != wxDataPaths.cend(); cit++)
				wxDataPathsComboBox->addItem(QString::fromStdWString(*cit));
			wxDataPathsComboBox->setToolTip(wxDataPathsComboBox->currentText());
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
			wxidsComboBox->clear();
			wxDataPathsComboBox->clear();
			wxDataPathsComboBox->setToolTip(wxDataPathsComboBox->currentText());
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
	QHBoxLayout* wxidsLayout = nullptr;
	QLabel* wxidsHintLabel = nullptr;
	QComboBox* wxidsComboBox = nullptr;
	QHBoxLayout* wxDataPathsLayout = nullptr;
	QLabel* wxDataPathsHintLabel = nullptr;
	QComboBox* wxDataPathsComboBox = nullptr;
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

