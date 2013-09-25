#include "dbus_plugin.h"
#include "hmiproxy.h"
#include "sdlproxy.h"

#include <qqml.h>

#include "dbus_plugin.h"
#include "optional_argument.h"
#include "qt_dbus.h"

#include <QtDBus/QDBusConnection>

#include <QQmlListReference>
#include <QString>

void DbusPlugin::registerTypes(const char *uri)
{
    // @uri sdl.core.api
    qmlRegisterType<HmiProxy>(uri, 1, 0, "HMIProxy");
    qmlRegisterType<SdlProxy>(uri, 1, 0, "SDLProxy");

    RegisterDbusMetatypes();
    qDBusRegisterMetaType<OptionalArgument<int> >();
    qDBusRegisterMetaType<OptionalArgument<QString> >();
    qDBusRegisterMetaType<OptionalArgument<bool> >();
    qDBusRegisterMetaType<OptionalArgument<double> >();

    HmiProxy::api_adaptors_.Init(this);

    QDBusConnection::sessionBus().registerObject("/", this);
    QDBusConnection::sessionBus().registerService("com.ford.sdl.hmi");
}

