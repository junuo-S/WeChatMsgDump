#include "stdafx.h"

#include <QApplication>
#include <QDesktopWidget>

double dpiScaled(double value)
{
	static double px = []() -> double
		{
			auto d = qApp->desktop();
			return d->logicalDpiX() / 96.0;
		}();
	return px * value;
}

