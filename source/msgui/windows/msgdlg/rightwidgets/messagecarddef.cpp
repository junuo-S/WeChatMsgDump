#include "stdafx.h"

#include "messagecarddef.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>

#include "msgapplication.h"
#include "utils/utils.h"

MessagecardNormalWidgetBase::MessagecardNormalWidgetBase(const MessagePtr& message, QWidget* parent /*= nullptr*/)
	: MessageCardWidgetBase(message, parent)
{
}

MessagecardNormalWidgetBase::~MessagecardNormalWidgetBase()
{
}

void MessagecardNormalWidgetBase::initUI()
{
	if (!m_message)
		return;
	bool isSender = m_message->IsSender();
	m_mainHLayout = new QHBoxLayout(this);
	if (isSender)
		m_mainHLayout->setDirection(QBoxLayout::RightToLeft);
	m_headImageVLayout = new QVBoxLayout(this);
	m_headImageButton = new QPushButton(this);
	m_headImageButton->setObjectName("headImageButton");
	m_headImageButton->setFixedSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageButton->setIcon(QIcon(":/icon_svg/head-image-none.svg"));
	m_headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageButton->setFlat(true);
	m_headImageButton->setEnabled(false);
	m_headImageVLayout->addWidget(m_headImageButton);
	m_headImageVLayout->addStretch();
	m_mainHLayout->addLayout(m_headImageVLayout);

	m_msgVLayout = new QVBoxLayout(this);
	m_msgVLayout->setAlignment(Qt::AlignRight);
	m_timeLabel = new QLabel(utils::QDateTimeToString(QDateTime::fromSecsSinceEpoch(m_message->GetCreateTime())), this);
	m_timeLabel->setObjectName("timeLabel");
	m_msgVLayout->addWidget(m_timeLabel, 0, isSender ? Qt::AlignRight : Qt::AlignLeft);
	m_msgVLayout->addWidget(createMsgContentWidget(), 0, isSender ? Qt::AlignRight : Qt::AlignLeft);
	m_mainHLayout->addLayout(m_msgVLayout);
	m_mainHLayout->addStretch();

	ComPtr<IJCoreApplication> coreApp = msgApp ? msgApp->GetCoreApplication() : nullptr;
	ComPtr<IJMsgViewManager> msgViewMgr = coreApp ? coreApp->GetMsgViewManager() : nullptr;
	if (!msgViewMgr)
		return;
	m_headWxid = isSender ? msgViewMgr->GetSelfWxid() : m_message->GetStrTalker();
	if (m_headWxid.isEmpty())
		return;

	refreshHeadImage();
}

void MessagecardNormalWidgetBase::refreshHeadImage()
{
	ComPtr<IJCoreApplication> coreApp = msgApp ? msgApp->GetCoreApplication() : nullptr;
	ComPtr<IJMsgViewManager> msgViewMgr = coreApp ? coreApp->GetMsgViewManager() : nullptr;
	if (!msgViewMgr || m_headWxid.isEmpty() || !m_headImageButton)
		return;
	std::optional<ContactInfo> contact = msgViewMgr->GetContactInfo(m_headWxid);
	if (!contact || contact->m_headImage.isNull())
		return;
	m_headImageButton->setIcon(contact->m_headImage);
	m_headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
}
