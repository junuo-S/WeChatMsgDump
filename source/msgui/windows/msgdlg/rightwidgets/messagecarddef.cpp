#include "stdafx.h"

#include "messagecarddef.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>

#include "msgapplication.h"
#include "utils/utils.h"
#include "defines.h"

MessagecardNormalWidgetBase::MessagecardNormalWidgetBase(const MessagePtr& message, QWidget* parent /*= nullptr*/)
	: MessageCardWidgetBase(message, parent)
{
	ComPtr<IJCoreApplication> coreApp = msgApp ? msgApp->GetCoreApplication() : nullptr;
	m_spMsgViewMgr = coreApp ? coreApp->GetMsgViewManager() : nullptr;
	if (m_spMsgViewMgr)
		attachTo(m_spMsgViewMgr.Get());
	if (m_message && m_spMsgViewMgr)
	{
		const bool isSender = m_message->IsSender();
		m_headWxid = isSender ? m_spMsgViewMgr->GetSelfWxid() : m_message->GetStrTalker();
	}
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

	refreshHeadImage();
}

void MessagecardNormalWidgetBase::refreshHeadImage()
{
	if (!m_spMsgViewMgr || m_headWxid.isEmpty() || !m_headImageButton)
		return;
	std::optional<ContactInfo> contact = m_spMsgViewMgr->GetContactInfo(m_headWxid);
	if (!contact || contact->m_headImage.isNull())
		return;
	m_headImageButton->setIcon(contact->m_headImage);
	m_headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
}

STDMETHODIMP_(bool) MessagecardNormalWidgetBase::OnCoreEvent(IJCoreEvent* event)
{
	if (!event || event->Type() != EventType::Event_MsgView)
		return false;
	JMsgViewAsyncEvent* asyncEvent = dynamic_cast<JMsgViewAsyncEvent*>(event);
	if (!asyncEvent || asyncEvent->m_op != MsgViewOpType::Op_ContactHeadImageReady)
		return false;
	if (asyncEvent->m_extraData.value(STR_WXID).toString() != m_headWxid)
		return false;
	QMetaObject::invokeMethod(this, [this]() { refreshHeadImage(); }, Qt::QueuedConnection);
	return true;
}
