#ifndef DBUS_CONTROLLER_H
#define DBUS_CONTROLLER_H

#include <QDBusMessage>
#include <QObject>
#include <map>

typedef bool (*fillRoutine)(QDBusMessage&, const QVariantMap&);

class DBusController: public QObject {
    struct delayedReply {
        QDBusMessage message;
        fillRoutine fill;
    };
    Q_OBJECT
public:
    explicit DBusController(QObject *parent=0);
    void addMessage(const QDBusMessage& message, fillRoutine fill, int async_uid);
    Q_INVOKABLE void sendReply(QVariant asyncObject, QVariant data);
private:
    std::map<int, delayedReply> replies;

};

#endif // DBUS_CONTROLLER_H

// vim: set ts=4 sw=4 et:
