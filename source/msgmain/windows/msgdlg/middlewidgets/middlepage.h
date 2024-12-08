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
	void addSessionCard(const QString& wxid);

signals:
	void sigSessionClicked(SessionOverviewCard* session, const QString& wxid, const QString& remark);

private:
	void onSessionClicked(SessionOverviewCard* session, const QString& wxid, const QString& remark);
	struct Data;
	QScopedPointer<Data> data;
};
