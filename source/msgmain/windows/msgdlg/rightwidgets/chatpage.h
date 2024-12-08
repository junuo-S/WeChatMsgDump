#pragma once

#include <QWidget>

class JunuoBaseTitleBar;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QLabel;
class ChatPage : public QWidget
{
	Q_OBJECT
	using Base = QWidget;
public:
	ChatPage(QWidget* parent = nullptr);
	~ChatPage();
	void setCurrentChatTalker(const QString& wxid, const QString& remark);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void initUI();

	QString m_currentChatTalkerWxid;
	QVBoxLayout* m_mainVLayout = nullptr;
	JunuoBaseTitleBar* m_titleBar = nullptr;
	QLabel* m_chatTalkerInfoLabel = nullptr;

	QHBoxLayout* m_searchHLayout = nullptr;
	QLineEdit* m_searchEdit = nullptr;
	QPushButton* m_searchButton = nullptr;
};
