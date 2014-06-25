#ifndef REQUEST_TO_SDL_H
#define REQUEST_TO_SDL_H

#include <QDBusConnection>
#include <QJSValue>
#include <QObject>
#include <QDBusInterface>

#include <hmi_requests.h>

class RequestToSDL : public QObject
{
    Q_OBJECT
  public:
    explicit RequestToSDL(QObject *parent = 0) {
        QDBusConnection bus = QDBusConnection::sessionBus();
        interface2 = new QDBusInterface("org.freedesktop.DBus",
                                       "/",
                                       "org.freedesktop.DBus",
                                       bus,
                                       this);
        interface1 = new QDBusInterface("org.xfce.PowerManager",
                                       "/org/xfce/PowerManager",
                                       "org.xfce.Power.Manager",
                                       bus,
                                       this);
        interface3 = new QDBusInterface("com.ford.sdl.core",
                                       "/",
                                       "com.ford.sdl.core.SDL",
                                       bus,
                                       this);
    }

    ~RequestToSDL() {
        interface1->deleteLater();
        interface2->deleteLater();
        interface3->deleteLater();

        this->deleteLater();
    }

    Q_INVOKABLE void Request1(QString name, QJSValue hmi_callback) {
        QList<QVariant> args;
        new HMIRequest_Request1(interface1, name, args, hmi_callback);
    }

    Q_INVOKABLE void Request2(QString name, QJSValue hmi_callback) {
        QList<QVariant> args;
        new HMIRequest_Request2(interface2, name, args, hmi_callback);
    }

    Q_INVOKABLE void Request3(QString name, QJSValue hmi_callback) {
        QList<QVariant> args;
        args << QVariant(7);
        new HMIRequest_Request3(interface3, name, args, hmi_callback);
    }

  private:
    QDBusInterface *interface1;
    QDBusInterface *interface2;
    QDBusInterface *interface3;

};

#endif // REQUEST_TO_SDL_H


