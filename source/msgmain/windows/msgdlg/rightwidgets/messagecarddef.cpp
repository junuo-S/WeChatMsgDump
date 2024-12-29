#include "stdafx.h"

#include "messagecarddef.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include <msgcore/databus/databus.h>

#include "dbparser/MSGParser.h"

MessagecardNormalWidgetBase::~MessagecardNormalWidgetBase()
{
	DATA_BUS_INSTANCE->detachHeadImageObserver(m_msgParser.getIsSender() ? DATA_BUS_INSTANCE->getWxid() : m_msgParser.getStrTalker(), this);
}

void MessagecardNormalWidgetBase::setHeadImage(const QPixmap& pixmap)
{
	if (m_headImageLabel)
		m_headImageLabel->setPixmap(pixmap);
}

void MessagecardNormalWidgetBase::initUI()
{
	m_mainHLayout = new QHBoxLayout(this);
	m_headImageVLayout = new QVBoxLayout(this);
	m_headImageLabel = new QLabel(this);
	m_headImageLabel->setFixedSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageLabel->setScaledContents(true);
	m_headImageVLayout->addWidget(m_headImageLabel);
	m_headImageVLayout->addStretch();
	m_mainHLayout->addLayout(m_headImageVLayout);

	m_msgVLayout = new QVBoxLayout(this);
	m_msgVLayout->addWidget(createMsgContentWidget());
	m_mainHLayout->addLayout(m_msgVLayout);
	m_mainHLayout->addStretch();

	DATA_BUS_INSTANCE->requestHeadImage(m_msgParser.getIsSender() ? DATA_BUS_INSTANCE->getWxid() : m_msgParser.getStrTalker(), this);
	if (m_msgParser.getIsSender())
		m_mainHLayout->setDirection(QBoxLayout::RightToLeft);
}
