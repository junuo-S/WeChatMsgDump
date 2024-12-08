#include "stdafx.h"

#include "chatpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include <junuoui/customwidget/junuobasetitlebar.h>

ChatPage::ChatPage(QWidget* parent /*= nullptr*/)
	: Base(parent)
	, m_mainVLayout(new QVBoxLayout(this))
	, m_titleBar(new JunuoBaseTitleBar(QPixmap(), QString(), this))
{
	initUI();
}

ChatPage::~ChatPage()
{

}

void ChatPage::setCurrentChatTalker(const QString& wxid, const QString& remark)
{
	m_currentChatTalkerWxid = wxid;
	m_chatTalkerInfoLabel->setText(remark);
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

	m_mainVLayout->addStretch();

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

