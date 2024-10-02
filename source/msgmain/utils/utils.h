#pragma once

#include <QIcon>

namespace utils
{
	QIcon CreateRoundedIcon(const QPixmap& originalPixmap, int radius = 4);
	QPixmap CreateRoundedPixmap(const QPixmap& originalPixmap, int radius = 20);
}
