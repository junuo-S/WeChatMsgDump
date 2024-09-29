#pragma once

#include <QWidget>
#include <QScopedPointer>

class SessionCardInfo;

class MiddlePage : public QWidget
{
	using Base = QWidget;
	Q_OBJECT
public:
	MiddlePage(Base* parent = nullptr);
	~MiddlePage();
	void addSessionCard(const QVariantList& infoList);
	void addSessionCard(const QVariantMap& cardInfo);
	void updateHeadImage(const QString& strTalker, const QPixmap& pixmap);
	void updateLastMessage(const QString& strTalker, const QString& msg);
	void updateLastMsgTime(const QString& strTalker, qint64 timestamp);

private:
	struct Data;
	QScopedPointer<Data> data;
};
