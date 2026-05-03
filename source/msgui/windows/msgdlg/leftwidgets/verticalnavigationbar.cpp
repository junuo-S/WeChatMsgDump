#include "stdafx.h"

#include "verticalnavigationbar.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QMouseEvent>
#include <QButtonGroup>

#include "msgapplication.h"
#include "utils/utils.h"
#include "defines.h"

constexpr static const char* const gs_strIndex = "index";

VerticalNavigationBar::VerticalNavigationBar(QWidget* parent /*= nullptr*/)
	: QFrame(parent)
{
	ComPtr<IJCoreApplication> coreApp = msgApp ? msgApp->GetCoreApplication() : nullptr;
	if (coreApp)
		m_spMsgViewMgr = coreApp->GetMsgViewManager();
	if (m_spMsgViewMgr)
		attachTo(m_spMsgViewMgr.Get());
	initUI();
}

VerticalNavigationBar::~VerticalNavigationBar()
{
}

void VerticalNavigationBar::startWork()
{
	if (!m_spMsgViewMgr)
		return;
	m_selfWxid = m_spMsgViewMgr->GetSelfWxid();
	if (m_selfWxid.isEmpty())
		return;
	refreshSelfHeadImage();
}

void VerticalNavigationBar::setCurrentPage(unsigned int index)
{
	QAbstractButton* button = m_radioButtonGroup->button(index);
	if (button)
		button->setChecked(true);
}

void VerticalNavigationBar::mousePressEvent(QMouseEvent* event)
{
	if (event && event->button() == Qt::LeftButton)
	{
		m_bLeftButtonDown = true;
		m_lastPos = event->pos();
	}
}

void VerticalNavigationBar::mouseReleaseEvent(QMouseEvent* event)
{
	if (event && event->button() == Qt::LeftButton)
		m_bLeftButtonDown = false;
}

void VerticalNavigationBar::mouseMoveEvent(QMouseEvent* event)
{
	if (!event || !m_bLeftButtonDown || !(event->buttons() & Qt::LeftButton))
		return;
	QPoint delta = event->pos() - m_lastPos;
	QWidget* dialog = window();
	if (dialog)
		dialog->move(dialog->pos() + delta);
}

void VerticalNavigationBar::onRadioButtonClicked(QAbstractButton* button)
{
	if (button)
		emit sigCurrentPageChanged(button->property(gs_strIndex).toUInt());
}

STDMETHODIMP_(bool) VerticalNavigationBar::OnCoreEvent(IJCoreEvent* event)
{
	if (!event || event->Type() != EventType::Event_MsgView)
		return false;
	JMsgViewAsyncEvent* asyncEvent = dynamic_cast<JMsgViewAsyncEvent*>(event);
	if (!asyncEvent)
		return false;
	if (asyncEvent->m_op == MsgViewOpType::Op_QueryContactInfo
		|| asyncEvent->m_op == MsgViewOpType::Op_ContactHeadImageReady)
	{
		if (asyncEvent->m_extraData.value(STR_WXID).toString() != m_selfWxid)
			return false;
		QMetaObject::invokeMethod(this, "onSelfContactInfoReady", Qt::QueuedConnection);
		return true;
	}
	return false;
}

void VerticalNavigationBar::onSelfContactInfoReady()
{
	if (!m_spMsgViewMgr || m_selfWxid.isEmpty())
		return;
	refreshSelfHeadImage();
}

void VerticalNavigationBar::refreshSelfHeadImage()
{
	if (!m_spMsgViewMgr || m_selfWxid.isEmpty())
		return;
	std::optional<ContactInfo> contact = m_spMsgViewMgr->GetContactInfo(m_selfWxid);
	if (!contact || contact->m_headImage.isNull())
		return;
	m_headImageButton->setIcon(contact->m_headImage);
	m_headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
}

void VerticalNavigationBar::initUI()
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
	setFixedWidth(DPI(56));
	setAutoFillBackground(true);
	m_mainVLayout = new QVBoxLayout(this);
	m_mainVLayout->setAlignment(Qt::AlignHCenter);
	m_mainVLayout->setContentsMargins(DPI(13), DPI(35), DPI(13), 0);
	m_headImageButton = new QPushButton(this);
	m_headImageButton->setObjectName("headImageButton");
	m_headImageButton->setFixedSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageButton->setIcon(utils::CreateRoundedIcon(QPixmap(":/icon_svg/head-image-none.svg").scaled(
		HEAD_IMAGE_ICON_SIZE, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation), DPI(6)));
	m_headImageButton->setIconSize(HEAD_IMAGE_ICON_SIZE);
	m_headImageButton->setCursor(Qt::PointingHandCursor);
	m_mainVLayout->addWidget(m_headImageButton);
	m_mainVLayout->addSpacing(DPI(20));

	m_radioButtonGroup = new QButtonGroup(this);
	VerticalNavigationBar::connect(m_radioButtonGroup, &QButtonGroup::buttonClicked, this, &VerticalNavigationBar::onRadioButtonClicked);
	m_msgRadioButton = new QRadioButton(this);
	m_msgRadioButton->setObjectName("msgRadioButton");
	m_msgRadioButton->setCursor(Qt::PointingHandCursor);
	m_msgRadioButton->setProperty(gs_strIndex, 0);
	m_radioButtonGroup->addButton(m_msgRadioButton, 0);
	m_mainVLayout->addWidget(m_msgRadioButton, 0, Qt::AlignHCenter);
	m_mainVLayout->addSpacing(DPI(12));

	m_friendRadioButton = new QRadioButton(this);
	m_friendRadioButton->setObjectName("friendRadioButton");
	m_friendRadioButton->setCursor(Qt::PointingHandCursor);
	m_friendRadioButton->setProperty(gs_strIndex, 1);
	m_radioButtonGroup->addButton(m_friendRadioButton, 1);
	m_mainVLayout->addWidget(m_friendRadioButton);

	m_mainVLayout->addStretch();
}
