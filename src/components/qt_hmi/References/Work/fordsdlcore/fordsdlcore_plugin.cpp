#include "fordsdlcore_plugin.h"
#include "smartdevicelink.h"
#include <QDebug>
#include <qqml.h>
#include "sdlalert.h"
#include <QQmlEngine>
#include <QQmlContext>

QObject* sdlSingleton(QQmlEngine * engine, QJSEngine * js) {
    Q_UNUSED(engine)
    Q_UNUSED(js)

    engine->rootContext()->setContextProperty("MediaApps", SmartDeviceLink::getInstance().getMediaApps());

    return (QObject *) &SmartDeviceLink::getInstance();
}

QObject* mediaAppsSingleton(QQmlEngine * engine, QJSEngine * js) {
    Q_UNUSED(engine)
    Q_UNUSED(js)

    return (QObject *) SmartDeviceLink::getInstance().getMediaApps();
}

void FordSdlCorePlugin::registerTypes(const char *uri)
{
    qmlRegisterSingletonType<SdlAppsListModel>(uri, 1, 0, "MediaApps", mediaAppsSingleton);
    qmlRegisterSingletonType<SmartDeviceLink>(uri, 1, 0, "AppLink", sdlSingleton);
}


