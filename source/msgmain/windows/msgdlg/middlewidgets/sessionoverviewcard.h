﻿#pragma once

#include <QFrame>
#include <QScopedPointer>

#include "msgcore/interface/interfacedecl.h"

class SessionOverviewCard : public QFrame, public IHeadImageObserver
{
	Q_OBJECT
public:
	SessionOverviewCard(const QString& wxid, QWidget* parent = nullptr);
	~SessionOverviewCard();
	void startWork();
	void setHeadImage(const QPixmap& pixmap) override;

private:
	Q_INVOKABLE void onSelectContactInfoFinished(const QVariantList& result, const QVariant& context = QVariant());
	Q_INVOKABLE void onSelectChatCountFinished(const QVariantList& result, const QVariant& context = QVariant());
	Q_INVOKABLE void onSelectLastMsgFinished(const QVariantList& result, const QVariant& context = QVariant());

	struct Data;
	QScopedPointer<Data> data;
};
