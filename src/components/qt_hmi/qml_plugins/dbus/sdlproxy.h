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
    void appUnregistered(int);
    void playTone();
    void showNotification(QVariant, QVariant, int);
private slots:
    void OnAppRegistered(Common_HMIApplication);
    void OnShowNotification(Common_TextFieldStruct text, OptionalArgument<Common_Image> image, int timeout);
};

QML_DECLARE_TYPE(SdlProxy)

#endif // SDLPROXY_H
