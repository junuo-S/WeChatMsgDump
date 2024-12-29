#include "stdafx.h"

#include "messagecard.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFontMetrics>

#include <msgcore/databus/databus.h>

#include "messagecardfactory.h"

void MessageCardPlantTextWidget::adjustBestSize()
{
	auto parentWidget = qobject_cast<QWidget*>(parent());
	if (!parentWidget)
		return;
	auto parentWidth = parentWidget->width();
	setFixedWidth(parentWidth * 0.98);
	int maxLabelWidth = parentWidth * 0.6;
	QFontMetrics metrics(m_msgContentLabel->font());
	int textWidth = metrics.horizontalAdvance(m_msgContentLabel->text()) + DPI(12);
	auto r = metrics.boundingRect(m_msgContentLabel->text());
	m_msgContentLabel->setFixedWidth(textWidth > maxLabelWidth ? maxLabelWidth : textWidth);
	if (textWidth > maxLabelWidth)
		m_msgContentLabel->setWordWrap(true);
}

QWidget* MessageCardPlantTextWidget::createMsgContentWidget()
{
	m_msgContentLabel = new QLabel(this);
	QFont font("SimSun", DPI(11));
	m_msgContentLabel->setFont(font);
	m_msgContentLabel->setContentsMargins(DPI(6), DPI(4), DPI(6), DPI(4));
	m_msgContentLabel->setObjectName(m_msgParser.getIsSender() ? "plantTextLabel_sender" : "plantTextLabel_receiver");
	m_msgContentLabel->setText(m_msgParser.getStrContent().remove("\r").remove("\n"));
	return m_msgContentLabel;
}

QWidget* MessageCardUnKnownWidget::createMsgContentWidget()
{
	m_msgContentLabel = new QLabel(this);
	QFont font("SimSun", DPI(11));
	m_msgContentLabel->setFont(font);
	m_msgContentLabel->setContentsMargins(DPI(6), DPI(4), DPI(6), DPI(4));
	m_msgContentLabel->setObjectName(m_msgParser.getIsSender() ? "plantTextLabel_sender" : "plantTextLabel_receiver");
	m_msgContentLabel->setText(tr("This Message is not support") + QString(": %1").arg(m_msgParser.getMsgType()));
	return m_msgContentLabel;
}

DECLAREFACTORY_MESSAGECARDWIDGET(MsgType::UnKnown, MessageCardUnKnownWidget)
DECLAREFACTORY_MESSAGECARDWIDGET(MsgType::PlantText, MessageCardPlantTextWidget)
