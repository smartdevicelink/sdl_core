#ifndef HMIPROXY_H
#define HMIPROXY_H

#include <QQuickItem>

#include "qt_dbus.h"

class HmiProxy : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(HmiProxy)
    
public:
    explicit HmiProxy(QQuickItem *parent = 0);
    static ApiAdaptors api_adaptors_;
protected:
    virtual void componentComplete();
};

QML_DECLARE_TYPE(HmiProxy)

#endif // HMIPROXY_H

