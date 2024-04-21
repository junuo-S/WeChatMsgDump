#pragma once

#include <QWidget>
#include <QScopedPointer>

class WxProcessListPage : public QWidget
{
	Q_OBJECT
public:
	WxProcessListPage(QWidget* parent);
	~WxProcessListPage();
	void resetContent();

private:
	struct Data;
	QScopedPointer<Data> data;
};
