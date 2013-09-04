/**
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

#include "hmi_message_handler/dbus_adapter.h"
#include <dbus/dbus.h>

namespace hmi_message_handler {

log4cxx::LoggerPtr DBusAdapter::logger_   =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("HMIMessageHandler"));

DBusAdapter::DBusAdapter(const std::string& serviceName,
                                              const std::string& path)
    : serviceName_(serviceName),
      path_(path),
      conn_(nullptr) {}

DBusAdapter::~DBusAdapter() {}

bool DBusAdapter::Init() {
  DBusError err;
  int ret;
  std::string str;
  dbus_error_init(&err);
  conn_ = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
    str = "DBus: Connection Error ";
    LOG4CXX_ERROR(logger_, str.append(err.message));
    dbus_error_free(&err);
    return false;
  }
  ret = dbus_bus_request_name(conn_, serviceName_.c_str(),
                              DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
  if (dbus_error_is_set(&err)) {
    str = "DBus: Name Error ";
    LOG4CXX_ERROR(logger_, str.append(err.message));
    dbus_error_free(&err);
    return false;
  }
  if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
    str = "DBus: Name Error ";
    LOG4CXX_ERROR(logger_, str.append(err.message));
    return false;
  }
  LOG4CXX_INFO(logger_, "DBus: Success init dbus adpator");
  return true;
}

void DBusAdapter::Send(const std::string& message) {
  if (conn_ == nullptr) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }
  dbus_uint32_t serial = 0;  // stub
  DBusMessage *msg;
  DBusMessageIter args;

  msg = dbus_message_new_signal("/dbus", serviceName_.c_str(), "receive");
  if (NULL == msg) {
    LOG4CXX_WARN(logger_, "DBus: Failed emit signal (Message Null)");
    return;
  }

  const char* value = message.c_str();
  dbus_message_iter_init_append(msg, &args);
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &value)) {
    LOG4CXX_WARN(logger_, "DBus: Failed emit signal (Out Of Memory)");
    return;
  }

  if (!dbus_connection_send(conn_, msg, &serial)) {
    LOG4CXX_WARN(logger_, "DBus: Failed emit signal (Out Of Memory)");
    return;
  }
  dbus_connection_flush(conn_);

  dbus_message_unref(msg);
  LOG4CXX_INFO(logger_, "DBus: Success emit signal");
}

bool DBusAdapter::Recv(std::string& message) {
  if (conn_ == nullptr) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return false;
  }
  DBusMessage* msg;
  dbus_connection_read_write(conn_, 0);
  msg = dbus_connection_pop_message(conn_);

  if (NULL != msg &&
      TRUE == dbus_message_is_method_call(msg, serviceName_.c_str(), "send")) {
    Reply(msg, conn_, message);
    dbus_message_unref(msg);
    LOG4CXX_INFO(logger_, message);
    return true;
  }
  return false;
}

void DBusAdapter::Reply(DBusMessage* msg, DBusConnection* conn,
                        std::string& message) {
  DBusMessageIter args;
  char* param;

  if (!dbus_message_iter_init(msg, &args)) {
    LOG4CXX_WARN(logger_, "DBus: Failed call method (Message has no arguments!)");
  } else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) {
    LOG4CXX_WARN(logger_, "DBus: Failed call method (Argument is not string!)");
  } else {
    dbus_message_iter_get_basic(&args, &param);
    message = param;
  }
}

}  // namespace hmi_message_handler

