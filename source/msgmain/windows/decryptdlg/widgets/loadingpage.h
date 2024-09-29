#pragma once

#include <QWidget>
#include <QScopedPointer>

class LoadingPage : public QWidget
{
	Q_OBJECT
public:
	LoadingPage(QWidget* parent);
	~LoadingPage();
	void startLoadingMovie();
	void stopLoadingMovie();

private:
	void onTimeOut();

	struct Data;
	QScopedPointer<Data> data;
};
