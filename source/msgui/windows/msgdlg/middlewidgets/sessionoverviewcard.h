#pragma once

#include <QFrame>
#include <global_interface.h>

class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class SessionOverviewCard : public QFrame, public IJCoreObserver
{
	Q_OBJECT
	Q_PROPERTY(QString selectedString READ getSelectedString)
public:
	SessionOverviewCard(const QString& wxid, QWidget* parent = nullptr);
	~SessionOverviewCard();
	void startWork();
	STDMETHODIMP_(bool) OnCoreEvent(IJCoreEvent* event) override;
	QString getTalkerWxid() const;
	QString getTalkerRemark() const;
	void setSelected(bool selected);
	QString getSelectedString() const;

signals:
	void sigSessionClicked(SessionOverviewCard* session, const QString& wxid, const QString& remark);

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	Q_INVOKABLE void onContactInfoReady();
	Q_INVOKABLE void onChatCountReady(quint64 count);
	Q_INVOKABLE void onLastMsgReady(const MessagePtr& message);
	void refreshHeadImage();
	void initUI();

	QPushButton* m_headImageButton = nullptr;
	QLabel* m_remarkLabel = nullptr;
	QLabel* m_lastMessageLabel = nullptr;
	QLabel* m_lastMsgTimeLabel = nullptr;
	QLabel* m_msgCountLabel = nullptr;
	QHBoxLayout* m_mainHLayout = nullptr;
	QVBoxLayout* m_middleVLayout = nullptr;
	QVBoxLayout* m_rightVLayout = nullptr;
	QString m_wxid;
	QString m_remark;
	bool m_isSelected = false;
	ComPtr<IJMsgViewManager> m_spMsgViewMgr;
};
