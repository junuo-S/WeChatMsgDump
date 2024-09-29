#pragma once

#include <functional>

#include "junuoui/customwidget/framelesswidget.h"

#include "global.h"

class WechatMsgDialog : public JunuoFrameLessWidget
{
	Q_OBJECT
public:
	WechatMsgDialog(QWidget* parent = nullptr);
	~WechatMsgDialog();
	void startWork();

private:
	void onCurrentPageChanged(unsigned int index);
	void initUI();
	void turnToPage(WechatPage page);
	void updateCurrentUserHeadImage();
	void selectHeadImageUrlByUserName(const QString& userName, const QVariant& context = QVariant());
	void selectSessionInfo();
	Q_INVOKABLE void onGotHeadImageUrl(QVariantList result, const QVariant& context = QVariant());
	Q_INVOKABLE void onSessionInfoReady(QVariantList result, const QVariant& context = QVariant());

	struct Data;
	std::unique_ptr<Data> data = nullptr;
};
