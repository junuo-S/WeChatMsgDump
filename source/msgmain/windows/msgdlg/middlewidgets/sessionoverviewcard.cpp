#include "stdafx.h"

#include "sessionoverviewcard.h"

#include <QDateTime>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "defines.h"
#include "msgcore/databus/databus.h"

constexpr static const char* const gs_strLoading = "loading...";

static QString QDateTimeToString(const QDateTime& dateTime)
{
	return dateTime.toString("yyyy/MM/dd hh:mm:ss");
}

struct SessionOverviewCard::Data
{
	void initUI()
	{
		q->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
		q->setFixedHeight(DPI(60));
		q->setMinimumWidth(DPI(230));
		mainHLayout = new QHBoxLayout(q);
		mainHLayout->setContentsMargins(DPI(8), DPI(8), DPI(8), DPI(8));
		headImageLabel = new QLabel(q);
		headImageLabel->setFixedSize(HEAD_IMAGE_ICON_SIZE);
		headImageLabel->setScaledContents(true);
		headImageLabel->setPixmap(QPixmap(":/icon_svg/head-image-none.svg"));
		mainHLayout->addWidget(headImageLabel);

		middleVLayout = new QVBoxLayout(q);
		middleVLayout->setContentsMargins(0, 0, 0, 0);
		remarkLabel = new QLabel(gs_strLoading, q);
		remarkLabel->setObjectName("remarkLabel");
		middleVLayout->addWidget(remarkLabel);
		lastMessageLabel = new QLabel(gs_strLoading, q);
		lastMessageLabel->setObjectName("lastMessageLabel");
		middleVLayout->addWidget(lastMessageLabel);
		mainHLayout->addLayout(middleVLayout, 10);

		rightVLayout = new QVBoxLayout(q);
		rightVLayout->setContentsMargins(0, 0, 0, 0);
		lastMsgTimeLabel = new QLabel(gs_strLoading, q);
		lastMsgTimeLabel->setObjectName("lastMsgTimeLabel");
		rightVLayout->addWidget(lastMsgTimeLabel);
		msgCountLabel = new QLabel(gs_strLoading, q);
		msgCountLabel->setObjectName("msgCountLabel");
		rightVLayout->addWidget(msgCountLabel);
		mainHLayout->addLayout(rightVLayout, 1);
	}

	SessionOverviewCard* q = nullptr;
	QLabel* headImageLabel = nullptr;
	QLabel* remarkLabel = nullptr;
	QLabel* lastMessageLabel = nullptr;
	QLabel* lastMsgTimeLabel = nullptr;
	QLabel* msgCountLabel = nullptr;
	QHBoxLayout* mainHLayout = nullptr;
	QVBoxLayout* middleVLayout = nullptr;
	QVBoxLayout* rightVLayout = nullptr;
	QString wxid;
};

SessionOverviewCard::SessionOverviewCard(const QString& wxid, QWidget* parent /*= nullptr*/)
	: QFrame(parent)
	, data(new Data)
{
	data->q = this;
	data->wxid = wxid;
	data->initUI();
}

SessionOverviewCard::~SessionOverviewCard()
{

}

void SessionOverviewCard::startWork()
{
	DATA_BUS_INSTANCE->attachHeadImageObserver(data->wxid, this);
	DATA_BUS_INSTANCE->requestHeadImage(data->wxid, this);
	DATA_BUS_INSTANCE->requestContactInfo(data->wxid, this, "onSelectContactInfoFinished");
	DATA_BUS_INSTANCE->requestChatCount(data->wxid, this, "onSelectChatCountFinished");
	DATA_BUS_INSTANCE->requestChatHistory(data->wxid, QDateTime::currentSecsSinceEpoch(), false, 1, this, "onSelectLastMsgFinished");
}

void SessionOverviewCard::setHeadImage(const QPixmap& pixmap)
{
	data->headImageLabel->setPixmap(pixmap);
}

Q_INVOKABLE void SessionOverviewCard::onSelectContactInfoFinished(const QVariantList& result, const QVariant& context /*= QVariant()*/)
{
	if (result.size() != 1)
	{
		data->remarkLabel->setText(data->wxid);
		return;
	}
	const auto& resultMap = result.at(0).toMap();
	const QString remark = resultMap.value(STR_REMARK).toString();
	const QString nickName = resultMap.value(STR_NICKNAME).toString();
	data->remarkLabel->setText(remark.isEmpty() ? nickName : remark);
}

Q_INVOKABLE void SessionOverviewCard::onSelectChatCountFinished(const QVariantList& result, const QVariant& context /*= QVariant()*/)
{
	if (result.size() != 1)
	{
		return;
	}
	const quint64 count = result.at(0).toMap().value(STR_CHATCOUNT).toULongLong();
	data->msgCountLabel->setText(QString::number(count));
}

Q_INVOKABLE void SessionOverviewCard::onSelectLastMsgFinished(const QVariantList& result, const QVariant& context /*= QVariant()*/)
{
	if (result.size() != 1)
	{
		return;
	}
	const auto& resultMap = result.at(0).toMap();
	qint64 createTime = resultMap.value(STR_CREATETIME).toLongLong();
	data->lastMsgTimeLabel->setText(QDateTimeToString(QDateTime::fromSecsSinceEpoch(createTime)));
	data->lastMessageLabel->setText(getMsgRecordSessionStr(resultMap));
}

QString SessionOverviewCard::getMsgRecordSessionStr(const QVariantMap& msg) const
{
	return msg.value(STR_STRCONTENT).toString();
}
