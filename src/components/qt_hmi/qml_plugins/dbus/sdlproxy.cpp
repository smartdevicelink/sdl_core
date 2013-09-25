#include "sdlproxy.h"

SdlProxy::SdlProxy(QQuickItem *parent)
    : QQuickItem(parent) {
    /*
    sdlBasicCommunicationInterface = new QDBusInterface("com.ford.sdl.core",
        "/",
        "com.ford.hmi.BasicCommunication",
        QDBusConnection::sessionBus(),
        this);
    if (sdlBasicCommunicationInterface)
    //*/
    QDBusConnection::systemBus().connect("com.ford.sdl.core", "/", "com.ford.hmi.BasicCommunication",
                                         "OnAppRegistered", this, SLOT(OnAppRegistered(Common_HMIApplication)));
}

void SdlProxy::OnAppRegistered(Common_HMIApplication app) {
    QVariantMap appMap;
    appMap["appId"] = QVariant::fromValue(app.appID);
    appMap["appName"] = QVariant::fromValue(app.appName);
    appMap["deviceName"] = QVariant::fromValue(app.deviceName);
    appMap["hmiDisplayLanguageDesired"] = QVariant::fromValue(app.hmiDisplayLanguageDesired);
    appMap["isMediaApplication"] = QVariant::fromValue(app.isMediaApplication);

    if (app.appType.presence) {
        QList<QVariant> appType;
        for(QList<int>::const_iterator it = app.appType.val.begin();
            it != app.appType.val.end();
            ++it) {
            appType.append(QVariant::fromValue(*it));
        }
        appMap["appType"] = appType;
    }

    if (app.ngnMediaScreenAppName.presence) {
        appMap["ngnMediaScreenAppName"] = QVariant::fromValue(app.ngnMediaScreenAppName.val);
    }

    if (app.icon.presence) {
        appMap["icon"] = QVariant::fromValue(app.icon.val);
    }

    emit appRegistered(QVariant(appMap));
}
