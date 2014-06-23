/**
 * \file dbus_controller.cpp
 * \brief DbusController class.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "dbus_controller.h"
#include "interfaces/QT_HMI_API.h"
#include <QtDBus/QDBusConnection>

DBusController::DBusController(QObject *parent)
    : QObject(parent),
      message(NULL) {}

void DBusController::addMessage(const QDBusMessage& message, fillRoutine fill,
                                int async_uid) {
  delayedReply reply;
  reply.message = message;
  reply.fill = fill;
  replies_[async_uid] = reply;
}

void DBusController::sendReply(QVariant asyncObject, QVariant data) {
  int uid = asyncObject.toMap()["__async_uid"].toInt();
  std::map<int, delayedReply>::iterator it = replies_.find(uid);
  if (it != replies_.end()) {
    QDBusMessage msg = it->second.message.createReply();
    if (!it->second.fill(msg, data.toMap())) {
        QDBusConnection::sessionBus()
            .send(it->second.message.createErrorReply(QDBusError::InternalError,
                QString::number(hmi_apis::Common_Result::INVALID_DATA)));
    } else {
        QDBusConnection::sessionBus().send(msg);
    }
    replies_.erase(it);
  }
}

void DBusController::sendReply(QVariant data) {
  if (!message)
    return;
  QDBusMessage msg = message->createReply();
  if (!fill(msg, data.toMap())) {
    QDBusConnection::sessionBus()
        .send(message->createErrorReply(QDBusError::InternalError,
             QString::number(hmi_apis::Common_Result::INVALID_DATA)));
  } else {
    QDBusConnection::sessionBus().send(msg);
  }
}

void DBusController::sendError(QVariant asyncObject, QVariant code, QVariant message) {
  int uid = asyncObject.toMap()["__async_uid"].toInt();
  std::map<int, delayedReply>::iterator it = replies_.find(uid);
  if (it != replies_.end()) {
    QDBusMessage msg = it->second.message.createErrorReply(code.toString(),
                                                           message.toString());
    QDBusConnection::sessionBus().send(msg);
    replies_.erase(it);
  }
}

void DBusController::sendError(QVariant asyncObject, QVariant code) {
  sendError(asyncObject, code, QVariant(""));
}
