#include "hmiproxy.h"
#include <QtDBus/QDBusConnection>

HmiProxy::HmiProxy(QQuickItem *parent):
    QQuickItem(parent) {
}

void HmiProxy::componentComplete() {
    QQuickItem::componentComplete();

    api_adaptors_.SetApi(this);
}

ApiAdaptors HmiProxy::api_adaptors_;
