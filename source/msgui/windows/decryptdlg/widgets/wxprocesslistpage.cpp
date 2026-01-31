#include "stdafx.h"

#include "wxprocesslistpage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "junuoui/button/buttons.h"

#include "msgapplication.h"

WxProcessListPage::WxProcessListPage(QWidget* parent)
	: QWidget(parent)
{
	initUI();
	initStyle();
	m_refreshLabel->installEventFilter(this);
	if (auto coreApplication = msgApp->GetCoreApplication(); auto manager = coreApplication ? coreApplication->GetDecryptManager() : nullptr)
		attachTo(manager);
}

WxProcessListPage::~WxProcessListPage()
{

}

void WxProcessListPage::startWork()
{
}

STDMETHODIMP_(bool) WxProcessListPage::OnCoreEvent(IJCoreEvent* event)
{
	if (event->Type() == EventType::Event_ReadProcess)
	{
		auto readProcessEvent = dynamic_cast<JCommonAsyncEvent*>(event);
		if (readProcessEvent && readProcessEvent->m_subType == JCommonAsyncEvent::SubType::SubType_End)
			QMetaObject::invokeMethod(this, "updateProcessInfo", Qt::QueuedConnection
				, Q_ARG(QString, readProcessEvent->m_extraData.value("version").toString())
				, Q_ARG(QString, readProcessEvent->m_extraData.value("executablePath").toString())
				, Q_ARG(qulonglong, readProcessEvent->m_extraData.value("processId").toULongLong())
				, Q_ARG(QString, readProcessEvent->m_extraData.value("nickName").toString())
				, Q_ARG(QString, readProcessEvent->m_extraData.value("userName").toString())
				, Q_ARG(QString, readProcessEvent->m_extraData.value("phoneNumber").toString())
				, Q_ARG(QString, readProcessEvent->m_extraData.value("wxid").toString())
				, Q_ARG(QString, readProcessEvent->m_extraData.value("dataPath").toString())
			);
	}
	return false;
}

bool WxProcessListPage::eventFilter(QObject* object, QEvent* event)
{
	if (object == m_refreshLabel && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent && mouseEvent->button() == Qt::LeftButton)
		{
			emit sigRefresh();
			return true;
		}
		return false;
	}
	return false;
}

void WxProcessListPage::showEvent(QShowEvent* event)
{
	m_beginButton->adjustBestSize();
	m_reuseLastRetButton->adjustBestSize();
}

Q_INVOKABLE void WxProcessListPage::updateProcessInfo(const QString& version, const QString& exePath, const qulonglong processId, const QString& nickName, const QString& userName, const QString& phoneNumber, const QString& wxid, const QString& dataPath)
{
	updateProcessInfoUI(version, exePath, processId, nickName, userName, phoneNumber, wxid, dataPath);
}

void WxProcessListPage::initUI()
{
	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->setContentsMargins(20, 0, 20, 4);

	m_tipHLayout = new QHBoxLayout(this);
	{
		m_tipLabel = new QLabel(this);
		m_tipLabel->setObjectName("title");
		m_tipLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
		m_refreshLabel = new QLabel(this);
		m_refreshLabel->setToolTip(WxProcessListPage::tr("refresh"));
		m_tipHLayout->addWidget(m_tipLabel);
		m_tipHLayout->addWidget(m_refreshLabel);
	}
	m_tipHLayout->setAlignment(Qt::AlignCenter);
	m_mainLayout->addLayout(m_tipHLayout);
	m_mainLayout->addSpacing(DPI(12));

	m_wxVersionLayout = new QHBoxLayout(this);
	{
		m_wxVersionHintLabel = new QLabel(WxProcessListPage::tr("wechat version: "), this);
		m_wxVersionLabel = new QLabel(this);
		m_wxVersionLayout->addWidget(m_wxVersionHintLabel);
		m_wxVersionLayout->addWidget(m_wxVersionLabel);
	}
	m_wxVersionLayout->setStretch(0, 1);
	m_wxVersionLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_wxVersionLayout);

	m_wxExePathLayout = new QHBoxLayout(this);
	{
		m_wxExePathHintLabel = new QLabel(WxProcessListPage::tr("wechat exe: "), this);
		m_wxExePathLabel = new QLabel(this);
		m_wxExePathLayout->addWidget(m_wxExePathHintLabel);
		m_wxExePathLayout->addWidget(m_wxExePathLabel);
	}
	m_wxExePathLayout->setStretch(0, 1);
	m_wxExePathLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_wxExePathLayout);

	m_processInfoHLayout = new QHBoxLayout(this);
	{
		m_processHintLabel = new QLabel(WxProcessListPage::tr("processs id: "), this);
		m_processLabel = new QLabel(this);
		m_processInfoHLayout->addWidget(m_processHintLabel);
		m_processInfoHLayout->addWidget(m_processLabel);
	}
	m_processInfoHLayout->setStretch(0, 1);
	m_processInfoHLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_processInfoHLayout);

	m_nickNameHLayout = new QHBoxLayout(this);
	{
		m_nickNameHintLabel = new QLabel(WxProcessListPage::tr("nickName: "), this);
		m_nickNameLabel = new QLabel(this);
		m_nickNameHLayout->addWidget(m_nickNameHintLabel);
		m_nickNameHLayout->addWidget(m_nickNameLabel);
	}
	m_nickNameHLayout->setStretch(0, 1);
	m_nickNameHLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_nickNameHLayout);

	m_userNameHLayout = new QHBoxLayout(this);
	{
		m_userNameHintLabel = new QLabel(WxProcessListPage::tr("userName: "), this);
		m_userNameLabel = new QLabel(this);
		m_userNameHLayout->addWidget(m_userNameHintLabel);
		m_userNameHLayout->addWidget(m_userNameLabel);
	}
	m_userNameHLayout->setStretch(0, 1);
	m_userNameHLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_userNameHLayout);

	m_phoneNumberHLayout = new QHBoxLayout(this);
	{
		m_phoneNumberHintLabel = new QLabel(WxProcessListPage::tr("phone number: "), this);
		m_phoneNumberLabel = new QLabel(this);
		m_phoneNumberHLayout->addWidget(m_phoneNumberHintLabel);
		m_phoneNumberHLayout->addWidget(m_phoneNumberLabel);
	}
	m_phoneNumberHLayout->setStretch(0, 1);
	m_phoneNumberHLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_phoneNumberHLayout);

	m_wxidLayout = new QHBoxLayout(this);
	{
		m_wxidHintLabel = new QLabel(WxProcessListPage::tr("wxid: "), this);
		m_wxidLabel = new QLabel(this);
		m_wxidLayout->addWidget(m_wxidHintLabel);
		m_wxidLayout->addWidget(m_wxidLabel);
	}
	m_wxidLayout->setStretch(0, 1);
	m_wxidLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_wxidLayout);

	m_wxDataPathLayout = new QHBoxLayout(this);
	{
		m_wxDataPathHintLabel = new QLabel(WxProcessListPage::tr("wechat path: "), this);
		m_wxDataPathLabel = new QLabel(this);
		m_wxDataPathLayout->addWidget(m_wxDataPathHintLabel);
		m_wxDataPathLayout->addWidget(m_wxDataPathLabel);
	}
	m_wxDataPathLayout->setStretch(0, 1);
	m_wxDataPathLayout->setStretch(1, 3);
	m_mainLayout->addLayout(m_wxDataPathLayout);

	m_buttonLayout = new QHBoxLayout(this);
	{
		m_beginButton = new JunuoBaseButton(WxProcessListPage::tr("begin decrypt"), this);
		m_beginButton->setBgColor(0x409eff, 0x66b1ff, 0x3a8ee6, 0xa0cfff);
		m_beginButton->setTextColor(Qt::white);
		m_buttonLayout->addWidget(m_beginButton);
		connect(m_beginButton, &QPushButton::clicked, this, &WxProcessListPage::sigStartDecrypt);

		m_reuseLastRetButton = new JunuoBaseButton(WxProcessListPage::tr("reuse last result begin"), this);
		m_reuseLastRetButton->setBgColor(0x67c23a, 0x85ce61, 0x5daf34, 0xb3e19d);
		m_reuseLastRetButton->setTextColor(Qt::white);
		m_buttonLayout->addWidget(m_reuseLastRetButton);
		connect(m_reuseLastRetButton, &QPushButton::clicked, this, &WxProcessListPage::sigReuseLastResultBeginMsgView);
	}
	m_mainLayout->addSpacing(DPI(10));
	m_mainLayout->addLayout(m_buttonLayout);
	m_mainLayout->addStretch();
}

void WxProcessListPage::initStyle()
{
	m_refreshLabel->setPixmap(QPixmap(":/icon_svg/refresh.svg").scaled(DPI_SIZE(20, 20)));
	m_refreshLabel->setCursor(Qt::PointingHandCursor);
}

void WxProcessListPage::updateProcessInfoUI(const QString& version, const QString& exePath, const qulonglong processId, const QString& nickName, const QString& userName, const QString& phoneNumber, const QString& wxid, const QString& dataPath)
{
	bool suc = !wxid.isEmpty() && !dataPath.isEmpty();
	QString readFailed = WxProcessListPage::tr("read failed");
	m_tipLabel->setText(suc ? WxProcessListPage::tr("read success tip") : WxProcessListPage::tr("read failed tip"));
	m_wxVersionLabel->setText(version.isEmpty() ? readFailed : version);
	m_wxExePathLabel->setText(exePath.isEmpty() ? readFailed : exePath);
	m_wxExePathLabel->setToolTip(m_wxExePathLabel->text());
	m_processLabel->setText(processId > 0 ? QString::number(processId) : readFailed);
	m_nickNameLabel->setText(nickName.isEmpty() ? readFailed : nickName);
	m_userNameLabel->setText(userName.isEmpty() ? readFailed : userName);
	m_phoneNumberLabel->setText(phoneNumber.isEmpty() ? readFailed : phoneNumber);
	m_wxidLabel->setText(suc ? wxid : readFailed);
	m_wxDataPathLabel->setText(suc ? dataPath : readFailed);
	m_wxDataPathLabel->setToolTip(m_wxDataPathLabel->text());
	m_beginButton->setEnabled(suc);
}

