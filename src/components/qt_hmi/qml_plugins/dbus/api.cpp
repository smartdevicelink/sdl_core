#include "api.h"
#include <QtDBus/QDBusConnection>

Api::Api(QQuickItem *parent):
    QQuickItem(parent)
{
    notifications = new OrgFreedesktopNotificationsInterface("org.naquadah.awesome.awful", "/", QDBusConnection::sessionBus(), this);
}

void Api::componentComplete()
{
    QQuickItem::componentComplete();

    buttonsAdaptor->setButtonsApi(findChild<QQuickItem*>("buttons"));
}

void Api::send(QString text)
{
    notifications->Notify("test", 1000, "", text, text, QStringList(), QMap<QString, QVariant>(), 5000);
}

ButtonsAdaptor *Api::buttonsAdaptor = NULL;
