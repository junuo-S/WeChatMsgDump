#pragma once

#include <QWidget>
#include <QScopedPointer>

class DecryptDialog : public QWidget
{
	Q_OBJECT
public:
	DecryptDialog(QWidget* parent);
	~DecryptDialog();
	void gotoLoadingPage();
	void gotoWxProcessListPage();

signals:
	void sigRefresh();

private:
	void onPageChanged(int index);
	struct Data;
	QScopedPointer<Data> data;
};
