#include "stdafx.h"

#include "chatpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QDateTime>
#include <QScrollBar>
#include <QResizeEvent>

#include <junuoui/customwidget/junuobasetitlebar.h>
#include <msgcore/databus/databus.h>

#include "messagecard.h"
#include "messagecardfactory.h"

ChatPage::ChatPage(QWidget* parent /*= nullptr*/)
	: Base(parent)
	, m_mainVLayout(new QVBoxLayout(this))
	, m_titleBar(new JunuoBaseTitleBar(QPixmap(), QString(), this))
{
	initUI();
}

ChatPage::~ChatPage()
{
	for (auto& list : m_chatContentMap)
	{
		qDeleteAll(list);
		list.clear();
	}
}

void ChatPage::setCurrentChatTalker(const QString& wxid, const QString& remark)
{
	m_currentChatTalkerWxid = wxid;
	m_chatTalkerInfoLabel->setText(remark);
	clearChatContentLayout();
	m_chatContentMap.contains(wxid) ? addMessageCardWidgetByCache(wxid) : requestChatHistory(wxid, QDateTime::currentSecsSinceEpoch(), false, 50);
	m_needScrollToBottom = true;
}

void ChatPage::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen(QColor(0xcccccc));
	pen.setWidth(1);
	painter.setPen(pen);
	painter.drawLine(0, m_chatTalkerInfoLabel->geometry().bottom(), width(), m_chatTalkerInfoLabel->geometry().bottom());
	Base::paintEvent(event);
}

void ChatPage::resizeEvent(QResizeEvent* event)
{
	emit sigSizeChanged(event->size());
}

void ChatPage::initUI()
{
	m_mainVLayout->setContentsMargins(DPI(25), 0, 0, 0);
	m_titleBar->setTargetWidget(qobject_cast<QWidget*>(parent()->parent()));
	m_mainVLayout->addWidget(m_titleBar);
	m_chatTalkerInfoLabel = new QLabel(this);
	m_chatTalkerInfoLabel->setObjectName("chatTalkerInfoLabel");
	m_chatTalkerInfoLabel->setFixedHeight(DPI(41));
	m_chatTalkerInfoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	m_mainVLayout->addWidget(m_chatTalkerInfoLabel);

	m_chatContentScrollArea = new QScrollArea(this);
	m_chatContentScrollArea->setObjectName("chatContentScrollArea");
	m_chatContentScrollArea->setWidgetResizable(true);
	m_chatContentScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	connect(m_chatContentScrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, &ChatPage::onChatContentScrollBarRangeChanged);
	connect(m_chatContentScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, &ChatPage::onChatContentScrollBarValueChanged);
	m_chatContentWidget = new QWidget(this);
	m_chatContentWidget->setObjectName("chatContentWidget");
	m_chatContentScrollArea->setWidget(m_chatContentWidget);
	m_chatContentVLayout = new QVBoxLayout(m_chatContentWidget);
	m_chatContentVLayout->setContentsMargins(0, 0, 0, 0);
	m_mainVLayout->addWidget(m_chatContentScrollArea);

	m_searchHLayout = new QHBoxLayout(this);
	m_searchEdit = new QLineEdit(this);
	m_searchEdit->setObjectName("searchEdit");
	m_searchEdit->setFixedHeight(DPI(35));
	m_searchEdit->setPlaceholderText(tr("type something to search"));
	m_searchButton = new QPushButton(this);
	m_searchButton->setObjectName("searchButton");
	m_searchButton->setFixedSize(DPI(80), DPI(35));
	m_searchButton->setText(tr("search"));
	m_searchHLayout->addWidget(m_searchEdit);
	m_searchHLayout->addWidget(m_searchButton);
	m_searchHLayout->setContentsMargins(0, 0, DPI(25), DPI(15));
	m_mainVLayout->addLayout(m_searchHLayout);
}

void ChatPage::clearChatContentLayout()
{
	if (!m_chatContentVLayout)
		return;
	QLayoutItem* item;
	while ((item = m_chatContentVLayout->takeAt(0)) != nullptr)
	{
		if (item->widget())
			item->widget()->setParent(nullptr);
		delete item;
	}
}

void ChatPage::scrollToBottom()
{
	m_chatContentScrollArea->verticalScrollBar()->setValue(m_chatContentScrollArea->verticalScrollBar()->maximum());
	m_scrollBarLastValue = m_chatContentScrollArea->verticalScrollBar()->maximum();
}

void ChatPage::onChatContentScrollBarRangeChanged(int min, int max)
{
	if (min >= max)
		return;
	if (m_needScrollToBottom)
	{
		scrollToBottom();
		m_needScrollToBottom = false;
	}
	if (m_scrollBarCurrentVisiabelDelta > 0)
		m_chatContentScrollArea->verticalScrollBar()->setValue(m_chatContentScrollArea->verticalScrollBar()->minimum() + m_scrollBarCurrentVisiabelDelta);
	else if (m_scrollBarCurrentVisiabelDelta < 0)
		m_chatContentScrollArea->verticalScrollBar()->setValue(m_chatContentScrollArea->verticalScrollBar()->maximum() + m_scrollBarCurrentVisiabelDelta);
	m_scrollBarCurrentVisiabelDelta = 0;
}

void ChatPage::onChatContentScrollBarValueChanged(int value)
{
	double verticalScrollPercentage = float(value - m_chatContentScrollArea->verticalScrollBar()->minimum()) / (m_chatContentScrollArea->verticalScrollBar()->maximum() - m_chatContentScrollArea->verticalScrollBar()->minimum());
	if (verticalScrollPercentage < 0.1 && m_scrollBarLastValue > value)
	{
		requestChatHistory(m_currentChatTalkerWxid, m_chatContentMap.value(m_currentChatTalkerWxid).front()->getCreateTime() - 1, false, 50);
		m_scrollBarCurrentVisiabelDelta = value - m_chatContentScrollArea->verticalScrollBar()->maximum();
	}
	else if (verticalScrollPercentage > 0.9 && m_scrollBarLastValue < value)
	{
		requestChatHistory(m_currentChatTalkerWxid, m_chatContentMap.value(m_currentChatTalkerWxid).back()->getCreateTime() + 1, true, 50);
		m_scrollBarCurrentVisiabelDelta = value - m_chatContentScrollArea->verticalScrollBar()->minimum();
	}
	m_scrollBarLastValue = value;
}

void ChatPage::addMessageCardWidgetByCache(const QString& wxid)
{
	for (const auto& messageCardWidget : m_chatContentMap.value(wxid))
	{
		m_chatContentVLayout->addWidget(messageCardWidget);
		messageCardWidget->adjustBestSize();
	}
}

void ChatPage::requestChatHistory(const QString& wxid, qint64 createTime, bool forward, size_t limit)
{
	if (m_isRequestingChatHistory)
		return;
	DATA_BUS_INSTANCE->requestChatHistory(wxid, createTime, forward, limit, this, "onRequestChatHistoryFinished");
	m_isRequestingChatHistory = true;
}

Q_INVOKABLE void ChatPage::onRequestChatHistoryFinished(const QVariantList& result, const QVariant& context /*= QVariant()*/)
{
	m_isRequestingChatHistory = false;
	if (result.isEmpty())
	{
		m_scrollBarCurrentVisiabelDelta = 0;
		return;
	}
	for (const auto& record : result)
	{
		auto messageCardWidget = MessageCardWidgetFactory::createInstance(record.toMap(), this);
		if (messageCardWidget->getStrTalker() != m_currentChatTalkerWxid)
		{
			messageCardWidget->deleteLater();
			break;
		}
		messageCardWidget->initUI();
		connect(this, &ChatPage::sigSizeChanged, messageCardWidget, &MessageCardWidgetBase::adjustBestSize, Qt::QueuedConnection);
		if (m_scrollBarCurrentVisiabelDelta <= 0)
		{
			m_chatContentVLayout->insertWidget(0, messageCardWidget);
			m_chatContentMap[m_currentChatTalkerWxid].push_front(messageCardWidget);
		}
		else
		{
			m_chatContentVLayout->addWidget(messageCardWidget);
			m_chatContentMap[m_currentChatTalkerWxid].push_back(messageCardWidget);
		}
		messageCardWidget->adjustBestSize();
	}
}
