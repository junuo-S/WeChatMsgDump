#pragma once

#include <QIconEngine>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QString>
#include <functional>

class JOnlineIconEngine : public QIconEngine
{
public:
    explicit JOnlineIconEngine(const QString& url = QString());
    ~JOnlineIconEngine() override = default;

    QIconEngine* clone() const override;
    void paint(QPainter* p, const QRect& rc, QIcon::Mode m, QIcon::State s) override;
    QPixmap pixmap(const QSize& sz, QIcon::Mode m, QIcon::State s) override;

    void startRequest();
    bool isReady() const;
    void setReadyCallback(std::function<void()> readyCallback);

private:
    QString m_url;
    std::function<void()> m_readyCallback;
    QPixmap m_headPixmap;
    bool m_requesting = false;
    QNetworkAccessManager* m_spNetworkManager = nullptr;
};
