#include "jonlineiconengine.h"

#include <QCoreApplication>
#include <QNetworkReply>
#include <QPainter>
#include <QUrl>

JOnlineIconEngine::JOnlineIconEngine(const QString& url)
    : m_url(url)
{
}

QIconEngine* JOnlineIconEngine::clone() const
{
    auto* engine = new JOnlineIconEngine(m_url);
    engine->m_headPixmap = m_headPixmap;
    engine->m_requesting = m_requesting;
    return engine;
}

void JOnlineIconEngine::paint(QPainter* p, const QRect& rc, QIcon::Mode m, QIcon::State s)
{
    Q_UNUSED(m)
    Q_UNUSED(s)
    QPixmap px = pixmap(rc.size(), m, s);
    if (!px.isNull())
        p->drawPixmap(rc, px);
}

QPixmap JOnlineIconEngine::pixmap(const QSize& sz, QIcon::Mode m, QIcon::State s)
{
    Q_UNUSED(m)
    Q_UNUSED(s)
    if (m_headPixmap.isNull())
        startRequest();
    if (m_headPixmap.isNull())
        return QPixmap(":/icon_svg/head-image-none.svg").scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return m_headPixmap.scaled(sz, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void JOnlineIconEngine::startRequest()
{
    if (m_requesting || m_url.isEmpty())
        return;

    if (!m_spNetworkManager)
        m_spNetworkManager = new QNetworkAccessManager(QCoreApplication::instance());

    m_requesting = true;
    QNetworkReply* reply = m_spNetworkManager->get(QNetworkRequest(QUrl(m_url)));
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]()
        {
            const QByteArray bytes = reply->readAll();
            QPixmap px;
            if (!bytes.isEmpty())
                px.loadFromData(bytes);
            if (!px.isNull())
                m_headPixmap = px;
            m_requesting = false;
            reply->deleteLater();
        });
}

bool JOnlineIconEngine::isReady() const
{
    return !m_headPixmap.isNull();
}
