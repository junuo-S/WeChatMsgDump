#include "utils.h"

#include <QPainter>
#include <QPainterPath>

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

