#pragma once

#include <memory>

#include "junuoui/customwidget/framelesswidget.h"

class WechatMsgDialog : public JunuoFrameLessWidget
{
	Q_OBJECT
public:
	WechatMsgDialog(QWidget* parent = nullptr);
	~WechatMsgDialog();
	void initUI();

private:
	struct Data;
	std::unique_ptr<Data> data = nullptr;
};
