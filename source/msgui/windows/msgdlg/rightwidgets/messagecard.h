#pragma once

#include <QWidget>

#include "messagecarddef.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class MessageCardPlantTextWidget : public MessagecardNormalWidgetBase
{
	using Base = MessagecardNormalWidgetBase;
	Q_OBJECT
public:
	MessageCardPlantTextWidget(const MessagePtr& message, QWidget* parent) : Base(message, parent) {};
	virtual ~MessageCardPlantTextWidget() override = default;
	void adjustBestSize() override;

protected:
	QWidget* createMsgContentWidget() override;
	QLabel* m_msgContentLabel = nullptr;
};

class MessageCardUnKnownWidget : public MessageCardPlantTextWidget
{
	using Base = MessageCardPlantTextWidget;
	Q_OBJECT
public:
	MessageCardUnKnownWidget(const MessagePtr& message, QWidget* parent) : Base(message, parent) {};
	virtual ~MessageCardUnKnownWidget() override = default;

protected:
	QWidget* createMsgContentWidget() override;
};
