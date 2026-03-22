#pragma once

#include <QWidget>
#include <QScopedPointer>

#include <global_interface.h>

class MessageCardWidgetBase : public QWidget
{
	Q_OBJECT
public:
	MessageCardWidgetBase(const MessagePtr& message, QWidget* parent = nullptr) : QWidget(parent), m_message(message) {};
	virtual ~MessageCardWidgetBase() = default;
	QString getStrTalker() const { return m_message ? m_message->GetStrTalker() : QString(); }
	qint64 getCreateTime() const { return m_message ? m_message->GetCreateTime() : 0; }
	virtual void adjustBestSize() = 0;
	virtual void initUI() = 0;

protected:
	MessagePtr m_message;
};

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QPushButton;
class MessagecardNormalWidgetBase : public MessageCardWidgetBase
{
	Q_OBJECT
public:
	MessagecardNormalWidgetBase(const MessagePtr& message, QWidget* parent = nullptr);
	virtual ~MessagecardNormalWidgetBase() override;

protected:
	void initUI() override;
	void refreshHeadImage();
	virtual QWidget* createMsgContentWidget() = 0;

	QHBoxLayout* m_mainHLayout = nullptr;
	QVBoxLayout* m_headImageVLayout = nullptr;
	QPushButton* m_headImageButton = nullptr;
	QVBoxLayout* m_msgVLayout = nullptr;
	QLabel* m_timeLabel = nullptr;
	QString m_headWxid;
};
