#include "dbus_plugin.h"
#include "api.h"

#include <qqml.h>

#include "dbus_plugin.h"
#include "optionalArgument.h"
#include "presetBankCapabilities.h"
#include "buttonCapabilities.h"
#include "buttonsadaptor.h"

#include <QtDBus/QDBusConnection>

#include <QDBusMetaType>

void DbusPlugin::registerTypes(const char *uri)
{
    // @uri sdl.core.api
    qmlRegisterType<DbusPlugin>(uri, 1, 0, "Api");

    qmlRegisterType<QQuickButtonCapabilities>();
    qmlRegisterType<QQuickPresetBankCapabilities>();

    qDBusRegisterMetaType<ButtonCapabilities>();
    qDBusRegisterMetaType<QList<ButtonCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<ButtonCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<QList<ButtonCapabilities> > >();

    qDBusRegisterMetaType<PresetBankCapabilities>();
    qDBusRegisterMetaType<OptionalArgument<PresetBankCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<PresetBankCapabilities> >();
    qDBusRegisterMetaType<OptionalArgument<QList<PresetBankCapabilities> > >();

    new ButtonsAdaptor(this);

    QDBusConnection::sessionBus().registerObject("/Test", this);
    QDBusConnection::sessionBus().registerService("com.ford.api");
}

