#pragma once

#include <QWidget>
#include <QScopedPointer>

#include <msgcore/interface/interfacedecl.h>

#include "dbparser/MSGParser.h"

class MessageCardWidgetBase : public QWidget
{
	Q_OBJECT
public:
	MessageCardWidgetBase(const MSGParser& parser, QWidget* parent = nullptr) : QWidget(parent), m_msgParser(parser) {};
	virtual ~MessageCardWidgetBase() = default;
	QString getStrTalker() const { return m_msgParser.getStrTalker(); }
	virtual void adjustBestSize() = 0;
	virtual void initUI() = 0;

protected:
	MSGParser m_msgParser;
};

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class MessagecardNormalWidgetBase : public MessageCardWidgetBase, public IHeadImageObserver
{
	Q_OBJECT
public:
	MessagecardNormalWidgetBase(const MSGParser& parser, QWidget* parent = nullptr) : MessageCardWidgetBase(parser, parent) {};
	virtual ~MessagecardNormalWidgetBase() override;
	void setHeadImage(const QPixmap& pixmap) override;

protected:
	void initUI() override;
	virtual QWidget* createMsgContentWidget() = 0;

	QHBoxLayout* m_mainHLayout = nullptr;
	QVBoxLayout* m_headImageVLayout = nullptr;
	QLabel* m_headImageLabel = nullptr;
	QVBoxLayout* m_msgVLayout = nullptr;
	QLabel* m_timeLabel = nullptr;
};
