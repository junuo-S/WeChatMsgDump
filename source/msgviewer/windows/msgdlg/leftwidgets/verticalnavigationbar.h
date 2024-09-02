#pragma once

#include <QFrame>
#include <QScopedPointer>

class QAbstractButton;

class VerticalNavigationBar : public QFrame
{
	Q_OBJECT
public:
	VerticalNavigationBar(QWidget* parent = nullptr);
	~VerticalNavigationBar();
	void setHeadImage(const QIcon& icon);
	void setCurrentPage(unsigned int index);

signals:
	void sigCurrentPageChanged(unsigned int index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	void onRadioButtonClicked(QAbstractButton* button);
	struct Data;
	QScopedPointer<Data> data;
};
