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

DBusAdapter::DBusAdapter(const std::string& sdlServiceName,
                         const std::string& sdlObjectPath,
                         const std::string& hmiServiceName,
                         const std::string& hmiObjectPath)
    : sdl_service_name_(sdlServiceName),
      sdl_object_path_(sdlObjectPath),
      hmi_service_name_(hmiServiceName),
      hmi_object_path_(hmiObjectPath),
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
  ret = dbus_bus_request_name(conn_, sdl_service_name_.c_str(),
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

  LOG4CXX_INFO(logger_, "DBus: Success init dbus adaptor");
  return true;
}

bool DBusAdapter::Process(smart_objects::SmartObject& obj) {
  if (conn_ == nullptr) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return false;
  }
  DBusMessage* msg;
  dbus_connection_read_write(conn_, 0);
  msg = dbus_connection_pop_message(conn_);
  if (NULL != msg) {
    switch (dbus_message_get_type(msg)) {
      case DBUS_MESSAGE_TYPE_METHOD_CALL: ProcessMethodCall(msg, obj); break;
      case DBUS_MESSAGE_TYPE_METHOD_RETURN: ProcessMethodReturn(msg, obj); break;
      case DBUS_MESSAGE_TYPE_ERROR: ProcessError(msg, obj); break;
      case DBUS_MESSAGE_TYPE_SIGNAL: ProcessSignal(msg, obj); break;
      default: return false;
    }
    dbus_message_unref(msg);
    return true;
  }
  return false;
}

void DBusAdapter::MethodReturn(uint id, smart_objects::SmartObject& obj) {
  if (conn_ == nullptr) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }

  DBusMessage *msg;
  // TODO(KKolodiy): pop msg from something

  std::string interface = dbus_message_get_interface(msg);
  std::string method; // TODO(KKolodiy): where get name of method?

  dbus_schema::MessageName name(interface, method);
  dbus_schema::MessageId m_id = schema_.getMessageId(name);
  if (m_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return;
  }

  DBusMessage* reply;
  DBusMessageIter args;

  dbus_uint32_t serial = id;
  char* param;

  reply = dbus_message_new_method_return(msg);

  SetArgs(reply, schema_.getListArgs(m_id, hmi_apis::messageType::response), obj);

  if (!dbus_connection_send(conn_, reply, &serial)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed return method (Can't send message)");
    return;
  }
  dbus_connection_flush(conn_);

  dbus_message_unref(reply);
  dbus_message_unref(msg);
}

void DBusAdapter::Error(uint id, const std::string& name,
                        const std::string& description) {
  DBusMessage *msg;
  // TODO(KKolodiy): pop msg from something

  DBusMessage *error;
  dbus_uint32_t serial = id;
  error = dbus_message_new_error(msg, name.c_str(), description.c_str());

  if (!dbus_connection_send(conn_, error, &serial)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed send error (Can't send message)");
    return;
  }
  dbus_connection_flush(conn_);

  dbus_message_unref(error);
  dbus_message_unref(msg);
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

void DBusAdapter::MethodCall(uint id, const std::string& interface,
                             const std::string& method,
                             smart_objects::SmartObject& obj) {
  if (conn_ == nullptr) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }

  dbus_schema::MessageName name(interface, method);
  dbus_schema::MessageId m_id = schema_.getMessageId(name);
  if (m_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return;
  }

  DBusMessage* msg;
  DBusPendingCall* pending;

  msg = dbus_message_new_method_call(hmi_service_name_.c_str(),
       hmi_object_path_.c_str(),
       interface.c_str(),
       method.c_str());
  if (NULL == msg) {
    LOG4CXX_WARN(logger_, "DBus: Failed call method (Message Null)");
    return;
  }

  SetArgs(msg, schema_.getListArgs(m_id, hmi_apis::messageType::request), obj);

  if (!dbus_connection_send_with_reply(conn_, msg, &pending, -1)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Can't send message)");
    return;
  }
  if (NULL == pending) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Pending Null)");
    return;
  }
  dbus_connection_flush(conn_);

  dbus_message_unref(msg);
  LOG4CXX_INFO(logger_, "DBus: Success call method");
}

void DBusAdapter::Signal(uint id, const std::string& interface,
                         const std::string& signal,
                         smart_objects::SmartObject& obj) {
  if (conn_ == nullptr) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }

  dbus_schema::MessageName name(interface, signal);
  dbus_schema::MessageId m_id = schema_.getMessageId(name);
  if (m_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return;
  }

  dbus_uint32_t serial = id;
  DBusMessage *msg;
  msg = dbus_message_new_signal(sdl_object_path_.c_str(),
                                interface.c_str(),
                                signal.c_str());
  if (NULL == msg) {
    LOG4CXX_WARN(logger_, "DBus: Failed emit signal (Message Null)");
    return;
  }

  SetArgs(msg, schema_.getListArgs(m_id, hmi_apis::messageType::notification), obj);

  if (!dbus_connection_send(conn_, msg, &serial)) {
    LOG4CXX_WARN(logger_, "DBus: Failed emit signal (Out Of Memory)");
    return;
  }
  dbus_connection_flush(conn_);

  dbus_message_unref(msg);
  LOG4CXX_INFO(logger_, "DBus: Success emit signal");
}

bool DBusAdapter::AddMatch(const std::string& rule) {
  DBusError err;
  dbus_bus_add_match(conn_, rule.c_str(), &err);
  dbus_connection_flush(conn_);
  if (dbus_error_is_set(&err)) {
    std::string str = "DBus: Failed add match rule";
    LOG4CXX_WARN(logger_, str.append(" (").append(err.message).append(")"));
    return false;
  }
  return true;
}

void DBusAdapter::ProcessMethodCall(DBusMessage* msg, smart_objects::SmartObject& obj) {
  std::string name = dbus_message_get_member(msg);
  LOG4CXX_INFO(logger_, "DBus: name of method" << name);

  // TODO(KKolodiy): push msg

//  if (NULL != msg &&
//      TRUE == dbus_message_is_method_call(msg, sdl_service_name_.c_str(), "send")) {
//    Reply(msg, conn_, message);
//    dbus_message_unref(msg);
//    LOG4CXX_INFO(logger_, message);
//    return true;
//  }
//  return false;


}

void DBusAdapter::ProcessMethodReturn(DBusMessage* msg, smart_objects::SmartObject& obj) {
}

void DBusAdapter::ProcessError(DBusMessage* msg, smart_objects::SmartObject& obj) {
}

void DBusAdapter::ProcessSignal(DBusMessage* msg, smart_objects::SmartObject& obj) {
  std::string name = dbus_message_get_member(msg);
  LOG4CXX_INFO(logger_, "DBus: name of method" << name);

  // read the parameters
//   if (!dbus_message_iter_init(msg, &args))
//      fprintf(stderr, "Message has no arguments!\n");
//   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
//      fprintf(stderr, "Argument is not string!\n");
//   else {
//      dbus_message_iter_get_basic(&args, &sigvalue);
//      printf("Got Signal with value %s\n", sigvalue);
//   }
}

bool DBusAdapter::SetArgs(DBusMessage* msg,
                          const dbus_schema::ListArgs& rules,
                          smart_objects::SmartObject& obj) {
  DBusMessageIter args;

  dbus_message_iter_init_append(msg, &args);

  return true;
}

bool DBusAdapter::GetArgs(DBusMessage* msg) {
//DBusMessage* reply;
//   DBusMessageIter args;
//   DBusConnection* conn;
//   bool stat = true;
//   dbus_uint32_t level = 21614;
//   dbus_uint32_t serial = 0;
//   char* param = "";
//
//   // read the arguments
//   if (!dbus_message_iter_init(msg, &args))
//      fprintf(stderr, "Message has no arguments!\n");
//   else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
//      fprintf(stderr, "Argument is not string!\n");
//   else
//      dbus_message_iter_get_basic(&args, &param);
//   printf("Method called with %s\n", param);
  return true;
}

//bool DBusAdapter::SetBasicArg(DBusMessageIter* args,
//                              dbus_schema::ParameterDescription* rule,
//                              smart_objects::SmartObject& obj) {
//  int type = 0;
//  void* value;
//  dbus_int32_t integerValue;
//  dbus_bool_t booleanValue;
//  double floatValue;
//  const char* stringValue;
//  switch (rule->type) {
//    case ford_message_descriptions::Integer:
//      type = DBUS_TYPE_INT32;
//      integerValue = obj.asInt();
//      value = &integerValue;
//    break;
//    case ford_message_descriptions::Boolean:
//      type = DBUS_TYPE_BOOLEAN;
//      booleanValue = obj.asBool();
//      value = &booleanValue;
//    break;
//    case ford_message_descriptions::Float:
//      type = DBUS_TYPE_DOUBLE;
//      floatValue = obj.asDouble();
//      value = &floatValue;
//    break;
//    case ford_message_descriptions::String:
//      type = DBUS_TYPE_STRING;
//      stringValue = obj.asString().c_str();
//      value = &stringValue;
//    break;
//    default: {
//      LOG4CXX_WARN(logger_, "DBus: Failed set basic argument (Unknown type)");
//      return false;
//    }
//  }
//
//  if (rule->obligatory) {
//
//  }
//  if (!dbus_message_iter_append_basic(args, type, value)) {
//    LOG4CXX_WARN(logger_, "DBus: Failed set basic argument");
//    return false;
//  }
//  return true;
//}

const dbus_schema::DBusSchema& DBusAdapter::get_schema() const {
  return schema_;
}

//bool DBusAdapter::SetArrayArg(DBusMessageIter* args,
//                              dbus_schema::ParameterDescription* rule,
//                              smart_objects::SmartObject& obj) {
//  int type = 0;
//  void* value;
//  dbus_int32_t* integerValue;
//  dbus_bool_t* booleanValue;
//  double* floatValue;
//  const char** stringValue;
//  size_t size;
//  switch (rule->type) {
//    case ford_message_descriptions::Integer:
//      type = DBUS_TYPE_INT32;
//      size = ForeachBaseArray<dbus_int32_t>(obj, &integerValue);
//      value = &integerValue;
//    break;
//    case ford_message_descriptions::Boolean:
//      type = DBUS_TYPE_BOOLEAN;
//      size = ForeachBaseArray<dbus_bool_t>(obj, &booleanValue);
//      value = &booleanValue;
//    break;
//    case ford_message_descriptions::Float:
//      type = DBUS_TYPE_DOUBLE;
//      size = ForeachBaseArray<double>(obj, &floatValue);
//      value = &floatValue;
//    break;
//    case ford_message_descriptions::String:
//      type = DBUS_TYPE_STRING;
//      size = ForeachBaseArray<const char*>(obj, &stringValue);
//      value = &stringValue;
//    break;
//    default: {
//      LOG4CXX_WARN(logger_, "DBus: Failed set basic argument (Unknown type)");
//      return false;
//    }
//  }
//
//  if (!dbus_message_iter_append_fixed_array(args, type, value, size)) {
//    LOG4CXX_WARN(logger_, "DBus: Failed set basic argument");
//    return false;
//  }
//  return true;
//}

}  // namespace hmi_message_handler

