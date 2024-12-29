#pragma once

#include <QWidget>

class JunuoBaseTitleBar;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class MessageCardWidgetBase;
class ChatPage : public QWidget
{
	Q_OBJECT
	using Base = QWidget;
public:
	ChatPage(QWidget* parent = nullptr);
	~ChatPage();
	void setCurrentChatTalker(const QString& wxid, const QString& remark);

signals:
	void sigSizeChanged(const QSize& newSize);

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void initUI();
	void clearChatContentLayout();
	void addMessageCardWidgetByCache(const QString& wxid);
	Q_INVOKABLE void onRequestChatHistoryFinished(const QVariantList& result, const QVariant& context = QVariant());

	QString m_currentChatTalkerWxid;
	QVBoxLayout* m_mainVLayout = nullptr;
	JunuoBaseTitleBar* m_titleBar = nullptr;
	QLabel* m_chatTalkerInfoLabel = nullptr;
	QScrollArea* m_chatContentScrollArea = nullptr;
	QWidget* m_chatContentWidget = nullptr;
	QVBoxLayout* m_chatContentVLayout = nullptr;
	QMap<QString, QList<MessageCardWidgetBase*>> m_chatContentMap;

	QHBoxLayout* m_searchHLayout = nullptr;
	QLineEdit* m_searchEdit = nullptr;
	QPushButton* m_searchButton = nullptr;
};
