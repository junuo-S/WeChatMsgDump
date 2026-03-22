#include "stdafx.h"

#include "utils.h"

#include <QPainter>
#include <QPainterPath>
#include <QDateTime>

QIcon utils::CreateRoundedIcon(const QPixmap& originalPixmap, int radius /*= 4*/)
{
	QPixmap roundedPixmap(originalPixmap.size());
	roundedPixmap.fill(Qt::transparent);
	QPainter painter(&roundedPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath path;
	path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, originalPixmap);
	painter.end();
	return QIcon(roundedPixmap);
}

QPixmap utils::CreateRoundedPixmap(const QPixmap& originalPixmap, int radius /*= 20*/)
{
	QPixmap roundedPixmap(originalPixmap.size());
	roundedPixmap.fill(Qt::transparent);
	QPainter painter(&roundedPixmap);
	painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath path;
	path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, originalPixmap);
	painter.end();
	return roundedPixmap;
}

QString utils::QDateTimeToString(const QDateTime& dateTime)
{
	return dateTime.toString("yyyy/MM/dd hh:mm:ss");
}

QString utils::QDateTimeToStringOnlyDate(const QDateTime& dateTime)
{
	return dateTime.toString("yy/MM/dd");
}
