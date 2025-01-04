#include "stdafx.h"

#include "middlepage.h"

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMap>
#include <QPointer>
#include <QScrollBar>
#include <QTimer>

#include "global.h"
#include "sessionoverviewcard.h"

struct MiddlePage::Data
{
	void initUI()
	{
		q->setMinimumWidth(DPI(240));
		mainStackedLayout = new QStackedLayout(q);
		mainStackedLayout->setContentsMargins(0, 0, 0, 0);
		msgWidget = new QWidget(q);
		msgWidget->setObjectName("msgWidget");
		msgWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		msgScrollArea = new QScrollArea(q);
		msgScrollArea->setMinimumWidth(DPI(240));
		msgScrollArea->setWidgetResizable(true);
		msgScrollArea->setWidget(msgWidget);
		msgScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		MiddlePage::connect(msgScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, q, &MiddlePage::onSessionScrollAreaScrolled);
		MiddlePage::connect(msgScrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, q, &MiddlePage::onSessionScrollAreaScrolled);
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
		MiddlePage::connect(card, &SessionOverviewCard::sigSessionClicked, q, &MiddlePage::onSessionClicked);
		msgVLayout->addWidget(card);
		pendingSessionCards.append(card);
	}

	MiddlePage* q = nullptr;
	QStackedLayout* mainStackedLayout = nullptr;
	QWidget* msgWidget = nullptr;
	QScrollArea* msgScrollArea = nullptr;
	QVBoxLayout* msgVLayout = nullptr;
	QWidget* friendWidget = nullptr;
	QScrollArea* friendScrollArea = nullptr;
	QVBoxLayout* friendVLayout = nullptr;
	QPointer<SessionOverviewCard> currentSession = nullptr;
	QList<SessionOverviewCard*> pendingSessionCards;
	QTimer* refreshTimer = nullptr;
};

MiddlePage::MiddlePage(Base* parent /*= nullptr*/)
	: Base(parent)
	, data(new Data)
{
	data->q = this;
	data->refreshTimer = new QTimer(this);
	data->refreshTimer->setInterval(400);
	data->refreshTimer->setSingleShot(true);
	connect(data->refreshTimer, &QTimer::timeout, this, &MiddlePage::refreshSessionCardsInfo);
	data->initUI();
}

MiddlePage::~MiddlePage()
{

}

void MiddlePage::startWork()
{
	refreshSessionCardsInfo();
}

void MiddlePage::addSessionCard(const QString& wxid)
{
	data->addSessionCard(wxid);
}

void MiddlePage::onSessionClicked(SessionOverviewCard* session, const QString& wxid, const QString& remark)
{
	if (!session || session == data->currentSession)
		return;
	if (data->currentSession)
		data->currentSession->setSelected(false);
	data->currentSession = session;
	data->currentSession->setSelected(true);
	emit sigSessionClicked(wxid, remark);
}

void MiddlePage::onSessionScrollAreaScrolled(int value)
{
	data->refreshTimer->stop();
	data->refreshTimer->start();
}

void MiddlePage::refreshSessionCardsInfo()
{
	QRect visibleRect = data->msgScrollArea->viewport()->rect();
	QList<SessionOverviewCard*> needRemoveSessionCards;
	for (auto card : data->pendingSessionCards)
	{
		QRect cardRect = card->geometry();
		cardRect.moveTo(card->mapTo(data->msgScrollArea->viewport(), QPoint(0, 0)));
		if (visibleRect.intersects(cardRect))
		{
			card->startWork();
			needRemoveSessionCards.append(card);
		}
	}
	for (auto card : needRemoveSessionCards)
		data->pendingSessionCards.removeAll(card);
}
