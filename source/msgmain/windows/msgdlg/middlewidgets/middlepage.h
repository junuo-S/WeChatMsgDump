#pragma once

#include <QWidget>
#include <QScopedPointer>

class SessionCardInfo;

class MiddlePage : public QWidget
{
	using Base = QWidget;
	Q_OBJECT
public:
	MiddlePage(Base* parent = nullptr);
	~MiddlePage();
	void addSessionCard(const QString& wxid);

private:
	struct Data;
	QScopedPointer<Data> data;
};
