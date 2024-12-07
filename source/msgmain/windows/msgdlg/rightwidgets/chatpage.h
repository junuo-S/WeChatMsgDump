#pragma once

#include <QWidget>

class JunuoBaseTitleBar;
class QVBoxLayout;
class ChatPage : public QWidget
{
	Q_OBJECT
	using Base = QWidget;
public:
	ChatPage(QWidget* parent = nullptr);
	~ChatPage();

private:
	void initUI();
	QVBoxLayout* m_mainVLayout = nullptr;
	JunuoBaseTitleBar* m_titleBar = nullptr;
};
