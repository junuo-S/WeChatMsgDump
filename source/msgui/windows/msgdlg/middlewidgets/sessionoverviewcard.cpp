#include "stdafx.h"

#include "sessionoverviewcard.h"

#include <QDateTime>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyle>

#include "msgapplication.h"
#include "utils/utils.h"

#include "defines.h"

constexpr static const char* const gs_strLoading = "loading...";

SessionOverviewCard::SessionOverviewCard(const QString& wxid, QWidget* parent /*= nullptr*/)
	: QFrame(parent)
{
	m_wxid = wxid;
	initUI();
}

SessionOverviewCard::~SessionOverviewCard()
{
}

void SessionOverviewCard::startWork()
{
	if (!m_spMsgViewMgr)
	{
		ComPtr<IJCoreApplication> coreApp = msgApp ? msgApp->GetCoreApplication() : nullptr;
		if (coreApp)
			m_spMsgViewMgr = coreApp->GetMsgViewManager();
		if (m_spMsgViewMgr)
			attachTo(m_spMsgViewMgr.Get());
	}
	if (!m_spMsgViewMgr)
		return;

	std::optional<ContactInfo> contact = m_spMsgViewMgr->GetContactInfo(m_wxid);
	if (contact)
		onContactInfoReady();
	m_spMsgViewMgr->StartQueryMessageCount(m_wxid);
	m_spMsgViewMgr->StartQuerySessionLastMessage(m_wxid);
}

QString SessionOverviewCard::getTalkerWxid() const
{
	return m_wxid;
}

QString SessionOverviewCard::getTalkerRemark() const
{
	return m_remark;
}

void SessionOverviewCard::setSelected(bool selected)
{
	m_isSelected = selected;
	style()->unpolish(this);
	style()->polish(this);
}

QString SessionOverviewCard::getSelectedString() const
{
	return m_isSelected ? "selected" : "no-selected";
}

void SessionOverviewCard::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit sigSessionClicked(this, m_wxid, m_remarkLabel->text());
	}
}

STDMETHODIMP_(bool) SessionOverviewCard::OnCoreEvent(IJCoreEvent* event)
{
	if (!event || event->Type() != EventType::Event_MsgView)
		return false;
	JMsgViewAsyncEvent* asyncEvent = dynamic_cast<JMsgViewAsyncEvent*>(event);
	if (!asyncEvent)
		return false;

	const QVariantMap dataMap = asyncEvent->m_extraData;
	if (asyncEvent->m_op == MsgViewOpType::Op_QueryContactInfo)
	{
		if (dataMap.value(STR_WXID).toString() != m_wxid)
			return false;
		QMetaObject::invokeMethod(this, "onContactInfoReady", Qt::QueuedConnection);
		return true;
	}
	if (asyncEvent->m_op == MsgViewOpType::Op_ContactHeadImageReady)
	{
		if (dataMap.value(STR_WXID).toString() != m_wxid)
			return false;
		QMetaObject::invokeMethod(this, [this]() { refreshHeadImage(); }, Qt::QueuedConnection);
		return true;
	}
	if (asyncEvent->m_op == MsgViewOpType::Op_QueryMessageCount)
	{
		if (dataMap.value("talker").toString() != m_wxid)
			return false;
		QMetaObject::invokeMethod(this, "onChatCountReady", Qt::QueuedConnection, Q_ARG(quint64, dataMap.value("count").toULongLong()));
		return true;
	}
	if (asyncEvent->m_op == MsgViewOpType::Op_QuerySessionLastMessage)
	{
		if (dataMap.value("talker").toString() != m_wxid)
			return false;
		const QVariantList messages = dataMap.value("messages").toList();
		if (messages.isEmpty())
			return false;
		QMetaObject::invokeMethod(this, "onLastMsgReady", Qt::QueuedConnection, Q_ARG(MessagePtr, messages.first().value<MessagePtr>()));
		return true;
	}
	return false;
}

Q_INVOKABLE void SessionOverviewCard::onContactInfoReady()
{
	if (!m_spMsgViewMgr)
	{
		m_remarkLabel->setText(m_wxid);
		return;
	}

	std::optional<ContactInfo> contact = m_spMsgViewMgr->GetContactInfo(m_wxid);
	if (!contact)
	{
		m_remarkLabel->setText(m_wxid);
		return;
	}

	const QString remark = contact->m_remarkName;
	const QString nickName = contact->m_nickName;
	const QString display = remark.isEmpty() ? nickName : remark;
	m_remarkLabel->setText(m_remark = display.isEmpty() ? m_wxid : display);
	refreshHeadImage();
}

Q_INVOKABLE void SessionOverviewCard::onChatCountReady(quint64 count)
{
	m_msgCountLabel->setText(QString::number(count));
}

Q_INVOKABLE void SessionOverviewCard::onLastMsgReady(const MessagePtr& message)
{
	if (!message)
		return;
	m_lastMsgTimeLabel->setText(utils::QDateTimeToStringOnlyDate(QDateTime::fromSecsSinceEpoch(message->GetCreateTime())));
	m_lastMessageLabel->setText(message->GetDisplayText());
}

void SessionOverviewCard::refreshHeadImage()
{
	if (!m_spMsgViewMgr)
		return;
	std::optional<ContactInfo> contact = m_spMsgViewMgr->GetContactInfo(m_wxid);
	if (!contact || contact->m_headImage.isNull())
		return;
	m_headImageButton->setIcon(contact->m_headImage);
	m_headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
}

void SessionOverviewCard::initUI()
{
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
	setFixedHeight(DPI(60));
	setMinimumWidth(DPI(230));
	m_mainHLayout = new QHBoxLayout(this);
	m_mainHLayout->setContentsMargins(DPI(8), DPI(8), DPI(8), DPI(8));
	m_headImageButton = new QPushButton(this);
	m_headImageButton->setObjectName("headImageButton");
	m_headImageButton->setFixedSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageButton->setIcon(QIcon(":/icon_svg/head-image-none.svg"));
	m_headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageButton->setFlat(true);
	m_headImageButton->setEnabled(false);
	m_mainHLayout->addWidget(m_headImageButton);

	m_middleVLayout = new QVBoxLayout(this);
	m_middleVLayout->setContentsMargins(0, 0, 0, 0);
	m_remarkLabel = new QLabel(gs_strLoading, this);
	m_remarkLabel->setObjectName("remarkLabel");
	m_middleVLayout->addWidget(m_remarkLabel);
	m_lastMessageLabel = new QLabel(gs_strLoading, this);
	m_lastMessageLabel->setObjectName("lastMessageLabel");
	m_lastMessageLabel->setTextFormat(Qt::PlainText);
	m_middleVLayout->addWidget(m_lastMessageLabel);
	m_mainHLayout->addLayout(m_middleVLayout, 10);

	m_rightVLayout = new QVBoxLayout(this);
	m_rightVLayout->setContentsMargins(0, 0, 0, 0);
	m_lastMsgTimeLabel = new QLabel(gs_strLoading, this);
	m_lastMsgTimeLabel->setObjectName("lastMsgTimeLabel");
	m_rightVLayout->addWidget(m_lastMsgTimeLabel);
	m_msgCountLabel = new QLabel(gs_strLoading, this);
	m_msgCountLabel->setObjectName("msgCountLabel");
	m_rightVLayout->addWidget(m_msgCountLabel, 0, Qt::AlignRight);
	m_mainHLayout->addLayout(m_rightVLayout, 1);
}
