#include "api.h"
#include "api_interface.h"
#include <QtDBus/QDBusConnection>

Api::Api(QQuickItem *parent):
    QQuickItem(parent),
    name_(SdlCoreApiInterface::staticInterfaceName()),
    path_("/dbus"),
    api_(new SdlCoreApiInterface(name_, path_, QDBusConnection::sessionBus(), this))
{
    connect(api_, SIGNAL(receive(QString)), this, SIGNAL(receive(QString)));
}

Api::~Api()
{
    delete api_;
}

QString Api::name() const
{
    return name_;
}

QString Api::path() const
{
    return path_;
}

void Api::send(const QString &json)
{
    qDebug() << json;
    api_->send(json);
}

void Api::init()
{
    SdlCoreApiInterface *tmp = new SdlCoreApiInterface(name_, path_,
                                                       QDBusConnection::sessionBus(), this);
    if (tmp) {
        delete api_;
        api_ = tmp;
    }
}

void Api::setName(const QString &name)
{
    name_ = name;
    init();
    emit nameChanged();
}

void Api::setPath(const QString &path)
{
    path_ = path;
    init();
    emit pathChanged();
}

