#pragma once

#include <QIcon>
#include <QByteArray>

namespace utils
{
	QIcon CreateRoundedIcon(const QPixmap& originalPixmap, int radius = 4);
	QPixmap CreateRoundedPixmap(const QPixmap& originalPixmap, int radius = 20);
	bool DecompressLZ4(const QByteArray& compressedData, QByteArray& decompressedData);
	QString QDateTimeToString(const QDateTime& dateTime);
}
