#pragma once

#include <QWidget>
#include <QScopedPointer>

#include <global_interface.h>

class WxProcessListPage : public QWidget, public IJCoreObserver
{
	Q_OBJECT
public:
	WxProcessListPage(QWidget* parent);
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
	STDMETHODIMP_(bool) OnCoreEvent(IJCoreEvent* event) override;
	Q_INVOKABLE void updateProcessInfo(const QString& version, const QString& exePath, const qulonglong processId, const QString& nickName, const QString& userName, const QString& phoneNumber, const QString& wxid, const QString& dataPath);
	struct Data;
	QScopedPointer<Data> data;
};
