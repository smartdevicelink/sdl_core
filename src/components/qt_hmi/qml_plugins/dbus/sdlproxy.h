#ifndef SDLPROXY_H
#define SDLPROXY_H

#include <QQuickItem>
#include <QDBusInterface>

#include "qml_dbus.h"

class SdlProxy: public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(SdlProxy)

public:
    explicit SdlProxy(QQuickItem* parent = 0);
private:
    QDBusInterface *sdlBasicCommunicationInterface;
signals:
    void appRegistered(QVariant);
private slots:
    void OnAppRegistered(Common_HMIApplication);
};

QML_DECLARE_TYPE(SdlProxy)

#endif // SDLPROXY_H
