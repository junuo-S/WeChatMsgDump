#pragma once

#include <QWidget>
#include <QPointer>
#include <QSet>

class SessionOverviewCard;
class QStackedLayout;
class QScrollArea;
class QVBoxLayout;
class QHBoxLayout;
class QTimer;
class QLineEdit;
class QPushButton;

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
	void initUI();

	QStackedLayout* m_mainStackedLayout = nullptr;
	QWidget* m_msgPageWidget = nullptr;
	QVBoxLayout* m_msgPageVLayout = nullptr;
	QWidget* m_globalSearchWidget = nullptr;
	QHBoxLayout* m_globalSearchHLayout = nullptr;
	QLineEdit* m_globalSearchEdit = nullptr;
	QPushButton* m_globalSearchButton = nullptr;
	QWidget* m_msgWidget = nullptr;
	QScrollArea* m_msgScrollArea = nullptr;
	QVBoxLayout* m_msgVLayout = nullptr;
	QWidget* m_friendWidget = nullptr;
	QScrollArea* m_friendScrollArea = nullptr;
	QVBoxLayout* m_friendVLayout = nullptr;
	QPointer<SessionOverviewCard> m_currentSession = nullptr;
	QVector<SessionOverviewCard*> m_sessionCards;
	QSet<SessionOverviewCard*> m_startedSessionCards;
	QTimer* m_refreshTimer = nullptr;
};
