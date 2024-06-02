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

protected:
	bool eventFilter(QObject* object, QEvent* event) override;

signals:
	void sigRefresh();
	void sigStartDecrypt();

private:
	struct Data;
	QScopedPointer<Data> data;
};
