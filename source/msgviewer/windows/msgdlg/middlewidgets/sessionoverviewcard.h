#pragma once

#include <QWidget>
#include <QScopedPointer>

class SessionCardInfo
{
public:
	bool operator>(const SessionCardInfo& other) { return m_chatCount > other.m_chatCount; }
	bool operator>=(const SessionCardInfo& other) { return m_chatCount >= other.m_chatCount; }
	bool operator<(const SessionCardInfo& other) { return m_chatCount < other.m_chatCount; }
	bool operator<=(const SessionCardInfo& other) { return m_chatCount <= other.m_chatCount; }

	QString m_remark;
	QString m_nickName;
	QString m_alias;
	QString m_strTalker;
	size_t m_chatCount;
	QString m_lastMessage;
	qint64 m_lastMsgTime;
	QPixmap m_headImage;
};

class SessionOverviewCard : public QWidget
{
	using Base = QWidget;
	Q_OBJECT
public:
	SessionOverviewCard(const SessionCardInfo& cardInfo, Base* parent = nullptr);
	~SessionOverviewCard();
	void setHeadImage(const QPixmap& pixmap);
	void setLastMessage(const QString& msg);
	void setLastMsgTime(qint64 timestamp);

private:
	struct Data;
	QScopedPointer<Data> data;
};
