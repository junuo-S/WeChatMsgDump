#include "stdafx.h"

#include "msgdlg.h"

#include <QHBoxLayout>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCache>
#include <functional>

#include "dbreader/wechatdbreader.h"

#include "utils/utils.h"
#include "msgmanager/msgmanager.h"
#include "leftwidgets/verticalnavigationbar.h"

struct WechatMsgDialog::Data
{
	void initUI()
	{
		shellLayout = new QHBoxLayout(q);
		shellLayout->setContentsMargins(DPI(10), DPI(10), DPI(10), DPI(10));
		mainWidget = new QWidget(q);
		mainWidget->setObjectName("mainWidget");
		shellLayout->addWidget(mainWidget);
		mainHLayout = new QHBoxLayout(mainWidget);
		mainHLayout->setContentsMargins(0, 0, 0, 0);
		verticalNavigationBar = new VerticalNavigationBar(q);
		mainHLayout->addWidget(verticalNavigationBar);
		WechatMsgDialog::connect(verticalNavigationBar, &VerticalNavigationBar::sigCurrentPageChanged, q, &WechatMsgDialog::onCurrentPageChanged);
		mainHLayout->addStretch();

		QFile qssFile(":/stylesheet/wechatmsgdialog.qss");
		if (qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			q->setStyleSheet(qssFile.readAll());
		}
		qssFile.close();
	}

	WechatMsgDialog* q = nullptr;
	QLayout* shellLayout = nullptr;
	QWidget* mainWidget = nullptr;
	QHBoxLayout* mainHLayout = nullptr;
	VerticalNavigationBar* verticalNavigationBar = nullptr;

	QCache<QString, QIcon> cache;
	QNetworkAccessManager* networkManager = nullptr;
	WechatPage currentPage = ChatPage;
};

Q_DECLARE_METATYPE(std::function<void()>)

WechatMsgDialog::WechatMsgDialog(QWidget* parent /*= nullptr*/)
	: JunuoFrameLessWidget(parent)
	, data(new Data)
{
	data->q = this;
	data->networkManager = new QNetworkAccessManager(this);
	setWindowIcon(QIcon(":/icon_svg/wxchat.svg"));
}

WechatMsgDialog::~WechatMsgDialog()
{

}

void WechatMsgDialog::startWork()
{
	initUI();
	show();
	updateCurrentUserHeadImage();
	turnToPage(data->currentPage);
}

void WechatMsgDialog::onCurrentPageChanged(unsigned int index)
{
	data->currentPage = static_cast<WechatPage>(index);
	turnToPage(data->currentPage);
}

void WechatMsgDialog::initUI()
{
	data->initUI();
	setMinimumSize(DPI(680), DPI(560));
}

void WechatMsgDialog::turnToPage(WechatPage page)
{
	data->verticalNavigationBar->setCurrentPage(page);
}

void WechatMsgDialog::updateCurrentUserHeadImage()
{
	QIcon* icon = data->cache.object(MsgManager::instance()->getCurrentUserWxid());
	if (icon)
	{
		data->verticalNavigationBar->setHeadImage(*icon);
	}
	else
	{
		std::function<void()> callback = [this]() { updateCurrentUserHeadImage(); };
		selectHeadImageUrlByUserName(MsgManager::instance()->getCurrentUserWxid(), QVariant::fromValue(callback));
	}
}

void WechatMsgDialog::selectHeadImageUrlByUserName(const QString& userName, const QVariant& context /*= QVariant()*/)
{
	QVariantMap param;
	param.insert("userName", userName);
	MsgManager::instance()->getWechatDbReader()->selectHeadImageByUserName(this, "onGotHeadImageUrl", param, context);
}

void WechatMsgDialog::onGotHeadImageUrl(QVariantList result, const QVariant& context /*= QVariant()*/)
{
	if (result.size() != 1)
		return;
	QString userName = result.at(0).toMap().value("usrName").toString();
	QString smallUrl = result.at(0).toMap().value("smallHeadImgUrl").toString();
	if (smallUrl.isEmpty() || userName.isEmpty())
		return;
	QNetworkRequest requst(smallUrl);
	auto reply = data->networkManager->get(requst);
	connect(reply, &QNetworkReply::finished, this, [this, userName, context, reply]()
		{
			do 
			{
				if (reply->error() != QNetworkReply::NoError)
					break;
				QPixmap pixmap;
				if (!pixmap.loadFromData(reply->readAll()))
					break;
				data->cache.insert(userName, new QIcon(utils::CreateRoundedIcon(pixmap.scaled(DPI(30), DPI(30)))));
				if (!context.canConvert<std::function<void()>>())
					break;
				context.value<std::function<void()>>()();
			} while (false);
			reply->deleteLater();
		}
	);
}

