#include "stdafx.h"

#include "middlepage.h"

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMap>

#include "global.h"
#include "sessionoverviewcard.h"

struct MiddlePage::Data
{
	void initUI()
	{
		q->setMinimumWidth(DPI(230));
		mainStackedLayout = new QStackedLayout(q);
		mainStackedLayout->setContentsMargins(0, 0, 0, 0);
		msgWidget = new QWidget(q);
		msgWidget->setObjectName("msgWidget");
		msgWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		msgScrollArea = new QScrollArea(q);
		msgScrollArea->setMinimumWidth(DPI(230));
		msgScrollArea->setWidgetResizable(true);
		msgScrollArea->setWidget(msgWidget);
		msgScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		msgVLayout = new QVBoxLayout(msgWidget);
		msgVLayout->setContentsMargins(0, 0, 0, 0);
		msgVLayout->setSpacing(DPI(1));
		mainStackedLayout->addWidget(msgScrollArea);

		friendWidget = new QWidget(q);
		friendWidget->setObjectName("friendWidget");
		friendWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		friendScrollArea = new QScrollArea(q);
		friendScrollArea->setMinimumWidth(DPI(230));
		friendScrollArea->setWidgetResizable(true);
		friendScrollArea->setWidget(friendWidget);
		friendScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		friendVLayout = new QVBoxLayout(friendWidget);
		friendVLayout->setContentsMargins(0, 0, 0, 0);
		mainStackedLayout->addWidget(friendScrollArea);
	}

	void addSessionCard(const QString& wxid)
	{
		auto card = new SessionOverviewCard(wxid, msgWidget);
		card->startWork();
		msgVLayout->addWidget(card);
	}

	MiddlePage* q = nullptr;
	QStackedLayout* mainStackedLayout = nullptr;
	QWidget* msgWidget = nullptr;
	QScrollArea* msgScrollArea = nullptr;
	QVBoxLayout* msgVLayout = nullptr;
	QWidget* friendWidget = nullptr;
	QScrollArea* friendScrollArea = nullptr;
	QVBoxLayout* friendVLayout = nullptr;
	QMap<QString, SessionOverviewCard*> sessionCardTable;
};

MiddlePage::MiddlePage(Base* parent /*= nullptr*/)
	: Base(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
}

MiddlePage::~MiddlePage()
{

}

void MiddlePage::addSessionCard(const QString& wxid)
{
	data->addSessionCard(wxid);
}
