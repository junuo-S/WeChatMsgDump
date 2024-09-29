#pragma once

int dpiScaled(double value);

#define DPI(x) dpiScaled(x)
#define DPI_SIZE(x, y) QSize(dpiScaled(x), dpiScaled(y))

#define HEAD_IMAGE_ICON_SIZE DPI_SIZE(30, 30)
