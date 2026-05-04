#include "stdafx.h"

#include "middlepage.h"

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMap>
#include <QPointer>
#include <QScrollBar>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <algorithm>

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
	m_refreshTimer->stop();
	m_refreshTimer->start();
}

void MiddlePage::addSessionCard(const QString& wxid)
{
	SessionOverviewCard* card = new SessionOverviewCard(wxid, m_msgWidget);
	MiddlePage::connect(card, &SessionOverviewCard::sigSessionClicked, this, &MiddlePage::onSessionClicked);
	m_msgVLayout->addWidget(card);
	m_sessionCards.append(card);
	m_refreshTimer->stop();
	m_refreshTimer->start();
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
	if (!m_msgScrollArea || !m_msgScrollArea->viewport() || m_sessionCards.isEmpty())
		return;

	QScrollBar* scrollBar = m_msgScrollArea->verticalScrollBar();
	if (!scrollBar)
		return;

	const int cardHeight = DPI(60);
	const int stride = cardHeight + m_msgVLayout->spacing();
	if (stride <= 0)
		return;

	const int viewportTop = scrollBar->value();
	const int viewportBottom = viewportTop + m_msgScrollArea->viewport()->height();
	const int startIndex = std::max(0, viewportTop / stride - 1);
	const int lastIndex = static_cast<int>(m_sessionCards.size()) - 1;
	const int endIndex = std::min(lastIndex, viewportBottom / stride + 1);

	for (int i = startIndex; i <= endIndex; ++i)
	{
		SessionOverviewCard* card = m_sessionCards.at(i);
		if (!card || m_startedSessionCards.contains(card))
			continue;
		card->startWork();
		m_startedSessionCards.insert(card);
	}
}

void MiddlePage::initUI()
{
	setMinimumWidth(DPI(240));
	m_mainStackedLayout = new QStackedLayout(this);
	m_mainStackedLayout->setContentsMargins(0, 0, 0, 0);
	m_msgPageWidget = new QWidget(this);
	m_msgPageVLayout = new QVBoxLayout(m_msgPageWidget);
	m_msgPageVLayout->setContentsMargins(DPI(10), DPI(10), DPI(10), 0);
	m_msgPageVLayout->setSpacing(DPI(8));
	m_globalSearchWidget = new QWidget(m_msgPageWidget);
	m_globalSearchWidget->setObjectName("globalSearchWidget");
	m_globalSearchHLayout = new QHBoxLayout(m_globalSearchWidget);
	m_globalSearchHLayout->setContentsMargins(0, 0, 0, 0);
	m_globalSearchHLayout->setSpacing(DPI(6));
	m_globalSearchEdit = new QLineEdit(m_globalSearchWidget);
	m_globalSearchEdit->setObjectName("globalSearchEdit");
	m_globalSearchEdit->setPlaceholderText(tr("search in all chats"));
	m_globalSearchButton = new QPushButton(m_globalSearchWidget);
	m_globalSearchButton->setObjectName("globalSearchButton");
	m_globalSearchButton->setText(tr("Search"));
	m_globalSearchHLayout->addWidget(m_globalSearchEdit);
	m_globalSearchHLayout->addWidget(m_globalSearchButton);
	m_msgPageVLayout->addWidget(m_globalSearchWidget);

	m_msgWidget = new QWidget(m_msgPageWidget);
	m_msgWidget->setObjectName("msgWidget");
	m_msgWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_msgScrollArea = new QScrollArea(m_msgPageWidget);
	m_msgScrollArea->setMinimumWidth(DPI(240));
	m_msgScrollArea->setWidgetResizable(true);
	m_msgScrollArea->setWidget(m_msgWidget);
	m_msgScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	MiddlePage::connect(m_msgScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &MiddlePage::onSessionScrollAreaScrolled);
	MiddlePage::connect(m_msgScrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MiddlePage::onSessionScrollAreaScrolled);
	m_msgVLayout = new QVBoxLayout(m_msgWidget);
	m_msgVLayout->setContentsMargins(0, 0, 0, 0);
	m_msgVLayout->setSpacing(DPI(1));
	m_msgPageVLayout->addWidget(m_msgScrollArea);
	m_mainStackedLayout->addWidget(m_msgPageWidget);

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
