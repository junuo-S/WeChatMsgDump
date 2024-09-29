#pragma once

#include <QWidget>
#include <QScopedPointer>

class DecryptorWapper;

class LoadingPage : public QWidget
{
	Q_OBJECT
public:
	LoadingPage(DecryptorWapper* wapper, QWidget* parent);
	~LoadingPage();
	void startWork();

signals:
	void sigLoadingFinished();

private:
	void startLoadingMovie();
	void stopLoadingMovie();
	void onTimeOut();
	void onMemoryReadFinished(bool suc);

	struct Data;
	QScopedPointer<Data> data;
};
