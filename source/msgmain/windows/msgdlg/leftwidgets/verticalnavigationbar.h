#pragma once

#include <QFrame>
#include <QScopedPointer>

#include <msgcore/interface/interfacedecl.h>

class QAbstractButton;

class VerticalNavigationBar : public QFrame, public IHeadImageObserver
{
	Q_OBJECT
public:
	VerticalNavigationBar(QWidget* parent = nullptr);
	~VerticalNavigationBar();
	void startWork();
	void setCurrentPage(unsigned int index);
	void setHeadImage(const QPixmap& pixmap) override;

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
