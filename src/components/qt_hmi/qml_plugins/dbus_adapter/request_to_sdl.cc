#include <request_to_sdl.h>


RequestToSDL::RequestToSDL(QObject *parent) {
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

RequestToSDL::~RequestToSDL() {
    interface1->deleteLater();
    interface2->deleteLater();
    interface3->deleteLater();
    this->deleteLater();
}

void RequestToSDL::Request1(QString name, QJSValue hmi_callback) {
    QList<QVariant> args;
    new HMIRequest_Request1(interface1, name, args, hmi_callback);
}

void RequestToSDL::Request2(QString name, QJSValue hmi_callback) {
    QList<QVariant> args;
    new HMIRequest_Request2(interface2, name, args, hmi_callback);
}

void RequestToSDL::Request3(QString name, QJSValue hmi_callback) {
    QList<QVariant> args;
    args << QVariant(7);
    new HMIRequest_Request3(interface3, name, args, hmi_callback);
}









