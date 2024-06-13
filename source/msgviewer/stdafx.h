#pragma once

int dpiScaled(double value);

#define DPI(x) dpiScaled(x)
#define DPI_SIZE(x, y) QSize(dpiScaled(x), dpiScaled(y))
