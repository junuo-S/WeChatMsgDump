#pragma once

#include <QWidget>

#include <global_interface.h>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class JunuoBaseButton;

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
	void initUI();
	void initStyle();
	void updateProcessInfoUI(const QString& version, const QString& exePath, const qulonglong processId, const QString& nickName, const QString& userName, const QString& phoneNumber, const QString& wxid, const QString& dataPath);

	QVBoxLayout* m_mainLayout = nullptr;
	QHBoxLayout* m_tipHLayout = nullptr;
	QLabel* m_tipLabel = nullptr;
	QLabel* m_refreshLabel = nullptr;
	QHBoxLayout* m_wxVersionLayout = nullptr;
	QLabel* m_wxVersionHintLabel = nullptr;
	QLabel* m_wxVersionLabel = nullptr;
	QHBoxLayout* m_wxExePathLayout = nullptr;
	QLabel* m_wxExePathHintLabel = nullptr;
	QLabel* m_wxExePathLabel = nullptr;
	QHBoxLayout* m_processInfoHLayout = nullptr;
	QLabel* m_processHintLabel = nullptr;
	QLabel* m_processLabel = nullptr;
	QHBoxLayout* m_nickNameHLayout = nullptr;
	QLabel* m_nickNameHintLabel = nullptr;
	QLabel* m_nickNameLabel = nullptr;
	QHBoxLayout* m_userNameHLayout = nullptr;
	QLabel* m_userNameHintLabel = nullptr;
	QLabel* m_userNameLabel = nullptr;
	QHBoxLayout* m_phoneNumberHLayout = nullptr;
	QLabel* m_phoneNumberHintLabel = nullptr;
	QLabel* m_phoneNumberLabel = nullptr;
	QHBoxLayout* m_wxidLayout = nullptr;
	QLabel* m_wxidHintLabel = nullptr;
	QLabel* m_wxidLabel = nullptr;
	QHBoxLayout* m_wxDataPathLayout = nullptr;
	QLabel* m_wxDataPathHintLabel = nullptr;
	QLabel* m_wxDataPathLabel = nullptr;
	QHBoxLayout* m_buttonLayout = nullptr;
	JunuoBaseButton* m_beginButton = nullptr;
	JunuoBaseButton* m_reuseLastRetButton = nullptr;
};
