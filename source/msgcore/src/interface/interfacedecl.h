#pragma once

#include <comdef.h>
#include <QPixmap>

interface IHeadImageObserver
{
	virtual void setHeadImage(const QPixmap& pixmap) = 0;
};
