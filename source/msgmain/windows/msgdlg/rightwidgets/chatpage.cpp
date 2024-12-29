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
	m_chatContentMap.contains(wxid) ? addMessageCardWidgetByCache(wxid) : DATA_BUS_INSTANCE->requestChatHistory(wxid, QDateTime::currentSecsSinceEpoch(), false, 50, this, "onRequestChatHistoryFinished");
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

void ChatPage::addMessageCardWidgetByCache(const QString& wxid)
{
	for (const auto& messageCardWidget : m_chatContentMap.value(wxid))
	{
		m_chatContentVLayout->insertWidget(0, messageCardWidget);
		messageCardWidget->adjustBestSize();
	}
}

Q_INVOKABLE void ChatPage::onRequestChatHistoryFinished(const QVariantList& result, const QVariant& context /*= QVariant()*/)
{
	for (const auto& record : result)
	{
		auto messageCardWidget = MessageCardWidgetFactory::createInstance(record.toMap(), this);
		messageCardWidget->initUI();
		connect(this, &ChatPage::sigSizeChanged, messageCardWidget, &MessageCardWidgetBase::adjustBestSize, Qt::QueuedConnection);
		m_chatContentVLayout->insertWidget(0, messageCardWidget);
		messageCardWidget->adjustBestSize();
		m_chatContentMap[m_currentChatTalkerWxid].append(messageCardWidget);
	}
}

