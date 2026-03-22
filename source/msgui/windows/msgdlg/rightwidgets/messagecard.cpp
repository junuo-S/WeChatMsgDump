#include "stdafx.h"

#include "messagecard.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFontMetrics>

#include "messagecardfactory.h"

void MessageCardPlantTextWidget::adjustBestSize()
{
	QWidget* parentWidget = qobject_cast<QWidget*>(parent());
	if (!parentWidget)
		return;
	int parentWidth = parentWidget->width();
	setFixedWidth(parentWidth * 0.98);
	int maxLabelWidth = parentWidth * 0.6;
	QFontMetrics metrics(m_msgContentLabel->font());
	int textWidth = metrics.horizontalAdvance(m_msgContentLabel->text()) + DPI(12);
	QRect r = metrics.boundingRect(m_msgContentLabel->text());
	m_msgContentLabel->setFixedWidth(textWidth > maxLabelWidth ? maxLabelWidth : textWidth);
	if (textWidth > maxLabelWidth)
		m_msgContentLabel->setWordWrap(true);
}

QWidget* MessageCardPlantTextWidget::createMsgContentWidget()
{
	m_msgContentLabel = new QLabel(this);
	m_msgContentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
	QFont font("SimSun", DPI(11));
	m_msgContentLabel->setFont(font);
	m_msgContentLabel->setContentsMargins(DPI(6), DPI(4), DPI(6), DPI(4));
	const bool isSender = m_message ? m_message->IsSender() : false;
	m_msgContentLabel->setObjectName(isSender ? "plantTextLabel_sender" : "plantTextLabel_receiver");
	QString text = m_message ? m_message->GetDisplayText() : QString();
	m_msgContentLabel->setText(text.remove("\r").remove("\n"));
	return m_msgContentLabel;
}

QWidget* MessageCardUnKnownWidget::createMsgContentWidget()
{
	m_msgContentLabel = new QLabel(this);
	m_msgContentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
	QFont font("SimSun", DPI(11));
	m_msgContentLabel->setFont(font);
	m_msgContentLabel->setContentsMargins(DPI(6), DPI(4), DPI(6), DPI(4));
	const bool isSender = m_message ? m_message->IsSender() : false;
	m_msgContentLabel->setObjectName(isSender ? "plantTextLabel_sender" : "plantTextLabel_receiver");
	const int typeValue = m_message ? static_cast<int>(m_message->GetMsgType()) : -1;
	m_msgContentLabel->setText(tr("This Message is not support") + QString(": %1").arg(typeValue));
	return m_msgContentLabel;
}

DECLAREFACTORY_MESSAGECARDWIDGET(JMsgType::UnKnown, MessageCardUnKnownWidget)
DECLAREFACTORY_MESSAGECARDWIDGET(JMsgType::PlantText, MessageCardPlantTextWidget)
