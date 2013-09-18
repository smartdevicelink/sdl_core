#include "api.h"
#include <QtDBus/QDBusConnection>

Api::Api(QQuickItem *parent):
    QQuickItem(parent),
    path_("/dbus")
{
}

Api::~Api()
{
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
}

void Api::init()
{
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

void Api::componentComplete()
{
    QQuickItem::componentComplete();

    buttonsAdaptor->setButtonsApi(findChild<QQuickItem*>("buttons"));
}

ButtonsAdaptor *Api::buttonsAdaptor = NULL;
