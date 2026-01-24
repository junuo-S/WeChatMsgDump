#pragma once

#include <QWidget>
#include <QScopedPointer>

#include "junuoui/customwidget/junuoframelesswidget.h"

class DecryptDialog : public JunuoFrameLessWidget
{
	Q_OBJECT
public:
	DecryptDialog(QWidget* parent);
	~DecryptDialog();
	void startWork();

signals:
	void sigBeginMsgView();

private:
	void gotoLoadingPage();
	void gotoWxProcessListPage();
	void gotoDecryptingPage();
	void onPageChanged(int index);
	struct Data;
	QScopedPointer<Data> data;
};
