#pragma once

#include <QWidget>
#include <QScopedPointer>

class SessionOverviewCard;

class MiddlePage : public QWidget
{
	using Base = QWidget;
	Q_OBJECT
public:
	MiddlePage(Base* parent = nullptr);
	~MiddlePage();
	void startWork();
	void addSessionCard(const QString& wxid);

signals:
	void sigSessionClicked(const QString& wxid, const QString& remark);

private:
	void onSessionClicked(SessionOverviewCard* session, const QString& wxid, const QString& remark);
	void onSessionScrollAreaScrolled(int value);
	void refreshSessionCardsInfo();
	struct Data;
	QScopedPointer<Data> data;
};
