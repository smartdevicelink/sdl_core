#ifndef REQUEST_TO_SDL_H
#define REQUEST_TO_SDL_H

#include <QDBusConnection>
#include <QJSValue>
#include <QObject>
#include <QDBusInterface>

#include <hmi_requests.h>

class RequestToSDL : public QObject
{
    Q_OBJECT
  public:
    explicit RequestToSDL(QObject *parent = 0);

    ~RequestToSDL();

    Q_INVOKABLE void Request1(QString name, QJSValue hmi_callback);
    Q_INVOKABLE void Request2(QString name, QJSValue hmi_callback);
    Q_INVOKABLE void Request3(QString name, QJSValue hmi_callback);

  private:
    QDBusInterface *interface1;
    QDBusInterface *interface2;
    QDBusInterface *interface3;

};

#endif // REQUEST_TO_SDL_H


