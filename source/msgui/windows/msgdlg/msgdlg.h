#pragma once

#include <functional>

#include "junuoui/customwidget/junuoframelesswidget.h"

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
	Q_INVOKABLE void onSelectDistinctStrTalkerFinished(const QVariantList& result, const QVariant& context = QVariant());

	struct Data;
	std::unique_ptr<Data> data = nullptr;
};
