﻿#include "stdafx.h"

#include "middlepage.h"

#include <QStackedLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMap>

#include "windows/msgdlg/global.h"
#include "sessionoverviewcard.h"

struct MiddlePage::Data
{
	void initUI()
	{
		mainStackedLayout = new QStackedLayout(q);
		msgWidget = new QWidget(q);
		msgScrollArea = new QScrollArea(msgWidget);
		msgScrollArea->setWidget(msgWidget);
		msgVLayout = new QVBoxLayout(msgWidget);
		mainStackedLayout->addWidget(msgWidget);

		friendWidget = new QWidget(q);
		friendScrollArea = new QScrollArea(friendWidget);
		friendScrollArea->setWidget(friendWidget);
		friendVLayout = new QVBoxLayout(friendWidget);
		mainStackedLayout->addWidget(friendWidget);
	}

	void addSessionCard(const QVariantMap& cardInfoMap)
	{
		SessionCardInfo cardInfo;
		cardInfo.m_remark = cardInfoMap.value("Remark").toString();
		cardInfo.m_nickName = cardInfoMap.value("NickName").toString();
		cardInfo.m_alias = cardInfoMap.value("Alias").toString();
		cardInfo.m_strTalker = cardInfoMap.value("StrTalker").toString();
		cardInfo.m_chatCount = cardInfoMap.value("chatCount").toInt();
		cardInfo.m_lastMessage = cardInfoMap.value("lastMessage").toString();
		cardInfo.m_lastMsgTime = cardInfoMap.value("lastMsgTime").toUInt();

		SessionOverviewCard* card = new SessionOverviewCard(cardInfo, msgWidget);
		msgVLayout->addWidget(card);
		sessionCardTable.insert(cardInfo.m_strTalker, card);
	}

	void updateHeadImage(const QString& strTalker, const QPixmap& pixmap)
	{
		if (SessionOverviewCard* card = sessionCardTable.value(strTalker))
			card->setHeadImage(pixmap);
	}

	void updateLastMessage(const QString& strTalker, const QString& msg)
	{
		if (SessionOverviewCard* card = sessionCardTable.value(strTalker))
			card->setLastMessage(msg);
	}

	void updateLastMsgTime(const QString& strTalker, qint64 timestamp)
	{
		if (SessionOverviewCard* card = sessionCardTable.value(strTalker))
			card->setLastMsgTime(timestamp);
	}

	MiddlePage* q = nullptr;
	QStackedLayout* mainStackedLayout = nullptr;
	QWidget* msgWidget = nullptr;
	QScrollArea* msgScrollArea = nullptr;
	QVBoxLayout* msgVLayout = nullptr;
	QWidget* friendWidget = nullptr;
	QScrollArea* friendScrollArea = nullptr;
	QVBoxLayout* friendVLayout = nullptr;
	QMap<QString, SessionOverviewCard*> sessionCardTable;
};

MiddlePage::MiddlePage(Base* parent /*= nullptr*/)
	: Base(parent)
	, data(new Data)
{
	data->q = this;
	data->initUI();
}

MiddlePage::~MiddlePage()
{

}

void MiddlePage::addSessionCard(const QVariantList& infoList)
{
	for (const auto& info : infoList)
	{
		addSessionCard(info.toMap());
	}
}

void MiddlePage::addSessionCard(const QVariantMap& cardInfo)
{
	data->addSessionCard(cardInfo);
}

void MiddlePage::updateHeadImage(const QString& strTalker, const QPixmap& pixmap)
{
	data->updateHeadImage(strTalker, pixmap);
}

void MiddlePage::updateLastMessage(const QString& strTalker, const QString& msg)
{
	data->updateLastMessage(strTalker, msg);
}

void MiddlePage::updateLastMsgTime(const QString& strTalker, qint64 timestamp)
{
	data->updateLastMsgTime(strTalker, timestamp);
}
