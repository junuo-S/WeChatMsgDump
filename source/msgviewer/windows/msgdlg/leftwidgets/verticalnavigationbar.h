#pragma once

#include <QFrame>
#include <QScopedPointer>

class VerticalNavigationBar : public QFrame
{
	Q_OBJECT
public:
	VerticalNavigationBar(QWidget* parent = nullptr);
	~VerticalNavigationBar();
	void setHeadImage(const QIcon& icon);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	struct Data;
	QScopedPointer<Data> data;
};
