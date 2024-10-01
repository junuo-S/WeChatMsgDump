#pragma once

#ifndef DBREADER_EXPORT

#ifdef _DBREADER_MODULE_
#define DBREADER_EXPORT __declspec(dllexport)
#else
#define DBREADER_EXPORT __declspec(dllimport)
#endif // _DBREADER_MODULE_

#endif // DBREADER_EXPORT

#include <QObject>
#include <memory>
#include <QVariantMap>

class WechatDbReader : public QObject
{
	Q_OBJECT
public:
	WechatDbReader(const QString& dbName, QObject* parent = nullptr);
	~WechatDbReader();
	// 获取所有有聊天记录的微信号
	void selectAllStrTalkerFromMSG(QObject* receiver, const char* method, const QVariantMap& param = QVariantMap(), const QVariant& context = QVariant());
	// 根据userName获取头像链接
	void selectHeadImageByUserName(QObject* receiver, const char* method, const QVariantMap& param = QVariantMap(), const QVariant& context = QVariant());
	// 根据userName获取联系人信息
	void selectContactByUserName(QObject* receiver, const char* method, const QVariantMap& param = QVariantMap(), const QVariant& context = QVariant());
	// 获取会话列表（头像、最后的消息、最后消息的时间不在这里面）
	void selectAllSessionInfo(QObject* receiver, const char* method, const QVariantMap& param = QVariantMap(), const QVariant& context = QVariant());

private:
	struct Data;
	std::unique_ptr<Data> data = nullptr;
};
