#include "stdafx.h"

#include <QApplication>
#include <QScreen>

int dpiScaled(double value)
{
	static double px = []() -> double
		{
			auto screen = qApp->primaryScreen();
			return screen->logicalDotsPerInch() / 96.0;
		}();
	return px * value;
}

