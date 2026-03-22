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

MiddlePage::MiddlePage(Base* parent /*= nullptr*/)
	: Base(parent)
{
	m_refreshTimer = new QTimer(this);
	m_refreshTimer->setInterval(400);
	m_refreshTimer->setSingleShot(true);
	connect(m_refreshTimer, &QTimer::timeout, this, &MiddlePage::refreshSessionCardsInfo);
	initUI();
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
	SessionOverviewCard* card = new SessionOverviewCard(wxid, m_msgWidget);
	MiddlePage::connect(card, &SessionOverviewCard::sigSessionClicked, this, &MiddlePage::onSessionClicked);
	m_msgVLayout->addWidget(card);
	m_pendingSessionCards.append(card);
}

void MiddlePage::onSessionClicked(SessionOverviewCard* session, const QString& wxid, const QString& remark)
{
	if (!session || session == m_currentSession)
		return;
	if (m_currentSession)
		m_currentSession->setSelected(false);
	m_currentSession = session;
	m_currentSession->setSelected(true);
	emit sigSessionClicked(wxid, remark);
}

void MiddlePage::onSessionScrollAreaScrolled(int value)
{
	m_refreshTimer->stop();
	m_refreshTimer->start();
}

void MiddlePage::refreshSessionCardsInfo()
{
	QRect visibleRect = m_msgScrollArea->viewport()->rect();
	QList<SessionOverviewCard*> needRemoveSessionCards;
	for (SessionOverviewCard* card : m_pendingSessionCards)
	{
		QRect cardRect = card->geometry();
		cardRect.moveTo(card->mapTo(m_msgScrollArea->viewport(), QPoint(0, 0)));
		if (visibleRect.intersects(cardRect))
		{
			card->startWork();
			needRemoveSessionCards.append(card);
		}
	}
	for (SessionOverviewCard* card : needRemoveSessionCards)
		m_pendingSessionCards.removeAll(card);
}

void MiddlePage::initUI()
{
	setMinimumWidth(DPI(240));
	m_mainStackedLayout = new QStackedLayout(this);
	m_mainStackedLayout->setContentsMargins(0, 0, 0, 0);
	m_msgWidget = new QWidget(this);
	m_msgWidget->setObjectName("msgWidget");
	m_msgWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_msgScrollArea = new QScrollArea(this);
	m_msgScrollArea->setMinimumWidth(DPI(240));
	m_msgScrollArea->setWidgetResizable(true);
	m_msgScrollArea->setWidget(m_msgWidget);
	m_msgScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	MiddlePage::connect(m_msgScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &MiddlePage::onSessionScrollAreaScrolled);
	MiddlePage::connect(m_msgScrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MiddlePage::onSessionScrollAreaScrolled);
	m_msgVLayout = new QVBoxLayout(m_msgWidget);
	m_msgVLayout->setContentsMargins(0, 0, 0, 0);
	m_msgVLayout->setSpacing(DPI(1));
	m_mainStackedLayout->addWidget(m_msgScrollArea);

	m_friendWidget = new QWidget(this);
	m_friendWidget->setObjectName("friendWidget");
	m_friendWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_friendScrollArea = new QScrollArea(this);
	m_friendScrollArea->setMinimumWidth(DPI(230));
	m_friendScrollArea->setWidgetResizable(true);
	m_friendScrollArea->setWidget(m_friendWidget);
	m_friendScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_friendVLayout = new QVBoxLayout(m_friendWidget);
	m_friendVLayout->setContentsMargins(0, 0, 0, 0);
	m_mainStackedLayout->addWidget(m_friendScrollArea);
}
