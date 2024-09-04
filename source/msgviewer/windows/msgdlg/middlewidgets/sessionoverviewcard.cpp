#include "stdafx.h"

#include "sessionoverviewcard.h"

#include <QDateTime>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

constexpr static const char* const gs_strLoading = "loading...";

static QString QDateTimeToString(const QDateTime& dateTime)
{
	return dateTime.toString("yyyy/MM/dd hh:mm:ss");
}

struct SessionOverviewCard::Data
{
	void initUI()
	{
		q->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
		q->setFixedHeight(DPI(78));
		q->setMinimumWidth(DPI(230));
		mainHLayout = new QHBoxLayout(q);
		mainHLayout->setContentsMargins(DPI(8), DPI(12), DPI(8), DPI(12));
		headImageLabel = new QLabel(q);
		headImageLabel->setFixedSize(HEAD_IMAGE_ICON_SIZE);
		headImageLabel->setScaledContents(true);
		headImageLabel->setPixmap(cardInfo.m_headImage.isNull() ? QPixmap(":/icon_svg/head-image-none.svg") : cardInfo.m_headImage);
		mainHLayout->addWidget(headImageLabel);

		middleVLayout = new QVBoxLayout(q);
		middleVLayout->setContentsMargins(0, 0, 0, 0);
		remarkLabel = new QLabel(cardInfo.m_remark.isEmpty() ? cardInfo.m_nickName : cardInfo.m_remark, q);
		remarkLabel->setObjectName("remarkLabel");
		middleVLayout->addWidget(remarkLabel);
		lastMessageLabel = new QLabel(cardInfo.m_lastMessage.isEmpty() ? gs_strLoading : cardInfo.m_lastMessage, q);
		lastMessageLabel->setObjectName("lastMessageLabel");
		middleVLayout->addWidget(lastMessageLabel);
		mainHLayout->addLayout(middleVLayout);

		rightVLayout = new QVBoxLayout(q);
		rightVLayout->setContentsMargins(0, 0, 0, 0);
		lastMsgTimeLabel = new QLabel(cardInfo.m_lastMsgTime > 0 ? QDateTimeToString(QDateTime::fromSecsSinceEpoch(cardInfo.m_lastMsgTime)) : gs_strLoading, q);
		lastMsgTimeLabel->setObjectName("lastMsgTimeLabel");
		rightVLayout->addWidget(lastMsgTimeLabel);
		msgCountLabel = new QLabel(QString::number(cardInfo.m_chatCount), q);
		msgCountLabel->setObjectName("msgCountLabel");
		rightVLayout->addWidget(msgCountLabel);
		mainHLayout->addLayout(rightVLayout);
	}

	void updateHeadImage(const QPixmap& pixmap)
	{
		cardInfo.m_headImage = pixmap;
		headImageLabel->setPixmap(cardInfo.m_headImage.isNull() ? QPixmap(":/icon_svg/head-image-none.svg") : cardInfo.m_headImage);
	}

	void updateLastMessage(const QString& msg)
	{
		cardInfo.m_lastMessage = msg;
		lastMessageLabel->setText(cardInfo.m_lastMessage.isEmpty() ? gs_strLoading : cardInfo.m_lastMessage);
	}

	void updateLastMsgTime(qint64 timestamp)
	{
		cardInfo.m_lastMsgTime = timestamp;
		lastMsgTimeLabel->setText(cardInfo.m_lastMsgTime > 0 ? QDateTimeToString(QDateTime::fromSecsSinceEpoch(cardInfo.m_lastMsgTime)) : gs_strLoading);
	}

	SessionCardInfo cardInfo;
	SessionOverviewCard* q = nullptr;
	QLabel* headImageLabel = nullptr;
	QLabel* remarkLabel = nullptr;
	QLabel* lastMessageLabel = nullptr;
	QLabel* lastMsgTimeLabel = nullptr;
	QLabel* msgCountLabel = nullptr;
	QHBoxLayout* mainHLayout = nullptr;
	QVBoxLayout* middleVLayout = nullptr;
	QVBoxLayout* rightVLayout = nullptr;
};

SessionOverviewCard::SessionOverviewCard(const SessionCardInfo& cardInfo, Base* parent /*= nullptr*/)
	: Base(parent)
	, data(new Data)
{
	data->q = this;
	data->cardInfo = cardInfo;
	data->initUI();
}

SessionOverviewCard::~SessionOverviewCard()
{

}

void SessionOverviewCard::setHeadImage(const QPixmap& pixmap)
{
	data->updateHeadImage(pixmap);
}

void SessionOverviewCard::setLastMessage(const QString& msg)
{
	data->updateLastMessage(msg);
}

void SessionOverviewCard::setLastMsgTime(qint64 timestamp)
{
	data->updateLastMsgTime(timestamp);
}

