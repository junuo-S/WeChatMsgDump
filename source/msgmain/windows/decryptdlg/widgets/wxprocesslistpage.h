#pragma once

#include <QWidget>
#include <QScopedPointer>

class DecryptorWapper;

class WxProcessListPage : public QWidget
{
	Q_OBJECT
public:
	WxProcessListPage(DecryptorWapper* wapper, QWidget* parent);
	~WxProcessListPage();
	void startWork();

protected:
	bool eventFilter(QObject* object, QEvent* event) override;
	void showEvent(QShowEvent* event) override;

signals:
	void sigRefresh();
	void sigStartDecrypt();
	void sigReuseLastResultBeginMsgView();

private:
	void resetContent();
	struct Data;
	QScopedPointer<Data> data;
};
