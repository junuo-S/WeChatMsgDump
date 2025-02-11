﻿#include "stdafx.h"

#include "messagecarddef.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>

#include <msgcore/databus/databus.h>

#include "dbparser/MSGParser.h"
#include "utils/utils.h"

MessagecardNormalWidgetBase::MessagecardNormalWidgetBase(const MSGParser& parser, QWidget* parent /*= nullptr*/)
	: MessageCardWidgetBase(parser, parent)
{
	DATA_BUS_INSTANCE->attachHeadImageObserver(m_msgParser.getSenderWxid(), this);
}

MessagecardNormalWidgetBase::~MessagecardNormalWidgetBase()
{
	DATA_BUS_INSTANCE->detachHeadImageObserver(m_msgParser.getSenderWxid(), this);
}

void MessagecardNormalWidgetBase::setHeadImage(const QPixmap& pixmap)
{
	if (m_headImageLabel)
		m_headImageLabel->setPixmap(pixmap);
}

void MessagecardNormalWidgetBase::initUI()
{
	bool isSender = m_msgParser.getIsSender();
	m_mainHLayout = new QHBoxLayout(this);
	if (isSender)
		m_mainHLayout->setDirection(QBoxLayout::RightToLeft);
	m_headImageVLayout = new QVBoxLayout(this);
	m_headImageLabel = new QLabel(this);
	m_headImageLabel->setFixedSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageLabel->setScaledContents(true);
	m_headImageVLayout->addWidget(m_headImageLabel);
	m_headImageVLayout->addStretch();
	m_mainHLayout->addLayout(m_headImageVLayout);

	m_msgVLayout = new QVBoxLayout(this);
	m_msgVLayout->setAlignment(Qt::AlignRight);
	m_timeLabel = new QLabel(utils::QDateTimeToString(QDateTime::fromSecsSinceEpoch(m_msgParser.getCreateTime())), this);
	m_timeLabel->setObjectName("timeLabel");
	m_msgVLayout->addWidget(m_timeLabel, 0, isSender ? Qt::AlignRight : Qt::AlignLeft);
	m_msgVLayout->addWidget(createMsgContentWidget(), 0, isSender ? Qt::AlignRight : Qt::AlignLeft);
	m_mainHLayout->addLayout(m_msgVLayout);
	m_mainHLayout->addStretch();

	DATA_BUS_INSTANCE->requestHeadImage(m_msgParser.getSenderWxid(), this);
}
