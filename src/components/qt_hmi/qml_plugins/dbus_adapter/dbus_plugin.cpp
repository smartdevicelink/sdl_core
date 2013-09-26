#include "dbus_plugin.h"
#include "api.h"

#include <qqml.h>

#include "dbus_plugin.h"
#include "optional_argument.h"
#include "preset_bank_capabilities.h"
#include "button_capabilities.h"
#include "buttons_adaptor.h"

#include <QtDBus/QDBusConnection>

#include <QDBusMetaType>

#include <QQmlListReference>

void DbusPlugin::registerTypes(const char *uri)
{
    qDebug() << "void DbusPlugin::registerTypes(const char *uri)";
    // @uri sdl.core.api
    qmlRegisterType<Api>(uri, 1, 0, "Api");

    qDBusRegisterMetaType<ButtonCapabilities>();
    qDBusRegisterMetaType<QList<ButtonCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<ButtonCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<QList<ButtonCapabilities> > >();

    qDBusRegisterMetaType<PresetBankCapabilities>();
    qDBusRegisterMetaType<OptionalArgument<PresetBankCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<PresetBankCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<QList<PresetBankCapabilities> > >();

    Api::buttonsAdaptor = new ButtonsAdaptor(this);

    QDBusConnection::sessionBus().registerObject("/Test", this);
    QDBusConnection::sessionBus().registerService("com.ford.api");
}

