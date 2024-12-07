#include "stdafx.h"

#include "chatpage.h"

#include <QVBoxLayout>

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

void ChatPage::initUI()
{
	m_mainVLayout->setContentsMargins(0, 0, 0, 0);
	m_titleBar->setTargetWidget(qobject_cast<QWidget*>(parent()->parent()));
	m_mainVLayout->addWidget(m_titleBar);

	m_mainVLayout->addStretch();
}

