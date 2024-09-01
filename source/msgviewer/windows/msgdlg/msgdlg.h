#pragma once

#include <functional>

#include "junuoui/customwidget/framelesswidget.h"

class WechatMsgDialog : public JunuoFrameLessWidget
{
	Q_OBJECT
public:
	WechatMsgDialog(QWidget* parent = nullptr);
	~WechatMsgDialog();
	void startWork();

private:
	void initUI();
	void updateCurrentUserHeadImage();
	void selectHeadImageUrlByUserName(const QString& userName, const QVariant& context = QVariant());
	Q_INVOKABLE void onGotHeadImageUrl(QVariantList result, const QVariant& context = QVariant());

	struct Data;
	std::unique_ptr<Data> data = nullptr;
};
