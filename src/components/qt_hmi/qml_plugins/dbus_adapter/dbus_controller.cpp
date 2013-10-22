#include <QDBusConnection>
#include "dbus_controller.h"

DBusController::DBusController(QObject *parent)
    : QObject(parent) {
}

void DBusController::addMessage(const QDBusMessage& message, fillRoutine fill, int async_uid) {
    delayedReply reply;
    reply.message = message;
    reply.fill = fill;
    replies[async_uid] = reply;
}

void DBusController::sendReply(QVariant asyncObject, QVariant data) {
    int uid = asyncObject.toMap()["__async_uid"].toInt();
    std::map<int, delayedReply>::iterator it = replies.find(uid);
    if (it != replies.end()) {
        QDBusMessage msg = it->second.message.createReply();
        it->second.fill(msg, data.toMap());
        QDBusConnection::sessionBus().send(msg);
        replies.erase(it);
    }
}

void DBusController::sendError(QVariant asyncObject, QVariant data) {
    int uid = asyncObject.toMap()["__async_uid"].toInt();
    std::map<int, delayedReply>::iterator it = replies.find(uid);
    if (it != replies.end()) {
        QDBusMessage msg = it->second.message.createErrorReply(QDBusError::InternalError, data.toString());
        QDBusConnection::sessionBus().send(msg);
        replies.erase(it);
    }
}
// vim: set ts=4 sw=4 et:
