#include "sdlproxy.h"

SdlProxy::SdlProxy(QQuickItem *parent)
    : QQuickItem(parent) {

    QDBusConnection::systemBus().connect("com.ford.sdl.core", "/", "com.ford.hmi.BasicCommunication",
                                         "OnAppRegistered", this, SLOT(OnAppRegistered(Common_HMIApplication)));
    QDBusConnection::systemBus().connect("com.ford.sdl.core", "/", "com.ford.hmi.BasicCommunication",
                                         "OnAppUnregistered", this, SIGNAL(appUnregistered(int)));
    QDBusConnection::systemBus().connect("com.ford.sdl.core", "/", "com.ford.hmi.BasicCommunication",
                                         "PlayTone", this, SIGNAL(playTone()));
    QDBusConnection::systemBus().connect("com.ford.sdl.core", "/", "com.ford.hmi.UI",
                                         "ShowNotification", this, SLOT(OnShowNotification(Common_TextFieldStruct,OptionalArgument<Common_Image>,int)));
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

void SdlProxy::OnShowNotification(Common_TextFieldStruct text, OptionalArgument<Common_Image> image, int timeout) {
    QVariantMap txtMap;
    QVariant img;

    txtMap["fieldName"] = text.fieldName;
    txtMap["fieldText"] = text.fieldText;

    if (image.presence) {
        QVariantMap imgMap;
        imgMap["imageType"] = image.val.imageType;
        imgMap["value"] = image.val.value;
        img = QVariant(imgMap);
    }

    emit showNotification(QVariant(txtMap), img, timeout);
}
