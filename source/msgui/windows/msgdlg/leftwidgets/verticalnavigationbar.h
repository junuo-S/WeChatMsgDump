#pragma once

#include <QFrame>
#include <QPoint>
#include <global_interface.h>

class QAbstractButton;
class QVBoxLayout;
class QPushButton;
class QRadioButton;
class QButtonGroup;

class VerticalNavigationBar : public QFrame, public IJCoreObserver
{
	Q_OBJECT
public:
	VerticalNavigationBar(QWidget* parent = nullptr);
	~VerticalNavigationBar();
	void startWork();
	void setCurrentPage(unsigned int index);
	STDMETHODIMP_(bool) OnCoreEvent(IJCoreEvent* event) override;

signals:
	void sigCurrentPageChanged(unsigned int index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	Q_INVOKABLE void onSelfContactInfoReady();
	void refreshSelfHeadImage();
	void onRadioButtonClicked(QAbstractButton* button);

	void initUI();

	QVBoxLayout* m_mainVLayout = nullptr;
	QPushButton* m_headImageButton = nullptr;
	QRadioButton* m_msgRadioButton = nullptr;
	QRadioButton* m_friendRadioButton = nullptr;
	QButtonGroup* m_radioButtonGroup = nullptr;
	QPoint m_lastPos;
	bool m_bLeftButtonDown = false;

	QString m_selfWxid;
	ComPtr<IJMsgViewManager> m_spMsgViewMgr;
};
