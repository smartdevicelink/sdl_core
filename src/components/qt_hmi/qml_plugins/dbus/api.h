#ifndef API_H
#define API_H

#include <QQuickItem>

#include "buttonsadaptor.h"
#include "notify_interface.h"

class Api : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(Api)
    
public:
    explicit Api(QQuickItem *parent = 0);
    static ButtonsAdaptor* buttonsAdaptor;

    Q_INVOKABLE void send(QString text);

protected:
    virtual void componentComplete();

signals:

public slots:

private:
    OrgFreedesktopNotificationsInterface* notifications;
};

QML_DECLARE_TYPE(Api)

#endif // API_H

