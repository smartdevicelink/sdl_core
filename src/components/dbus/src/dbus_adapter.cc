/*
 * Copyright (c) 2013-2014, Ford Motor Company
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

#include "dbus/dbus_adapter.h"
#include <dbus/dbus.h>
#include <sstream>
#include "formatters/CSmartFactory.hpp"
#include "utils/logger.h"
#include "smart_objects/smart_object.h"

using ford_message_descriptions::ParameterDescription;
namespace sos = NsSmartDeviceLink::NsJSONHandler::strings;

extern char introspection_xml[];

namespace dbus {

CREATE_LOGGERPTR_GLOBAL(logger_, "HMIMessageHandler")

std::vector<std::string>& split(const std::string& s,
                                char delim,
                                std::vector<std::string>& elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

DBusAdapter::DBusAdapter(const std::string& sdlServiceName,
                         const std::string& sdlObjectPath,
                         const std::string& hmiServiceName,
                         const std::string& hmiObjectPath)
    : sdl_service_name_(sdlServiceName)
    , sdl_object_path_(sdlObjectPath)
    , hmi_service_name_(hmiServiceName)
    , hmi_object_path_(hmiObjectPath)
    , conn_(NULL)
    , schema_(new DBusSchema(ford_message_descriptions::message_descriptions)) {
}

DBusAdapter::~DBusAdapter() {
  dbus_shutdown();
  delete schema_;
}

bool DBusAdapter::Init() {
  DBusError err;
  int ret;
  dbus_error_init(&err);
  if (!dbus_threads_init_default()) {
    LOG4CXX_ERROR(logger_, "DBus: Can't initializes threads");
    return false;
  }
  conn_ = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
    LOG4CXX_ERROR(logger_, "DBus: Connection Error " << err.message);
    dbus_error_free(&err);
    return false;
  }
  ret = dbus_bus_request_name(
      conn_, sdl_service_name_.c_str(), DBUS_NAME_FLAG_DO_NOT_QUEUE, &err);
  if (ret == -1 || dbus_error_is_set(&err)) {
    LOG4CXX_ERROR(logger_, "DBus: Can't request name " << err.name);
    dbus_error_free(&err);
    return false;
  }
  if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
    LOG4CXX_ERROR(logger_,
                  "DBus: Service '" << sdl_service_name_
                                    << "' is already running");
    return false;
  }

  LOG4CXX_INFO(logger_, "DBus: Success init dbus adaptor");
  return true;
}

bool DBusAdapter::Process(smart_objects::SmartObject& obj) {
  if (conn_ == NULL) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't initialized");
    return false;
  }
  DBusMessage* msg = dbus_connection_pop_message(conn_);
  if (msg != NULL) {
    switch (dbus_message_get_type(msg)) {
      case DBUS_MESSAGE_TYPE_METHOD_CALL:
        return ProcessMethodCall(msg, obj);
      case DBUS_MESSAGE_TYPE_METHOD_RETURN:
        return ProcessMethodReturn(msg, obj);
      case DBUS_MESSAGE_TYPE_ERROR:
        return ProcessError(msg, obj);
      case DBUS_MESSAGE_TYPE_SIGNAL:
        return ProcessSignal(msg, obj);
      default:
        return false;
    }
  } else {
    dbus_connection_read_write(conn_, 50);
  }
  return false;
}

void DBusAdapter::MethodReturn(uint id,
                               const MessageId func_id,
                               const MessageName& name,
                               const smart_objects::SmartObject& obj) {
  LOG4CXX_DEBUG(logger_, "Method return " << name.first << "." << name.second);
  if (conn_ == NULL) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }

  if (func_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return;
  }

  DBusMessage* msg = GetRequestFromHMI(id);
  if (!msg) {
    LOG4CXX_WARN(logger_, "DBus: request from HMI is not found");
    return;
  }

  DBusMessage* reply;
  reply = dbus_message_new_method_return(msg);
  if (NULL == reply) {
    LOG4CXX_WARN(logger_, "DBus: Failed call method (Message Null)");
    return;
  }

  const ListArgs& args =
      schema_->getListArgs(func_id, hmi_apis::messageType::response);
  if (!SetArguments(reply, args, obj)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Signature is wrong)");
    dbus_message_unref(reply);
    dbus_message_unref(msg);
    return;
  }

  dbus_uint32_t serial;
  if (!dbus_connection_send(conn_, reply, &serial)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Can't send message)");
    dbus_message_unref(reply);
    dbus_message_unref(msg);
    return;
  }
  dbus_connection_flush(conn_);
  dbus_message_unref(reply);
  dbus_message_unref(msg);
  LOG4CXX_INFO(logger_, "DBus: Success return method");
}

void DBusAdapter::Error(uint id,
                        const std::string& name,
                        const std::string& description) {
  LOG4CXX_DEBUG(logger_, "Error " << name << ": " << description);
  if (conn_ == NULL) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }

  DBusMessage* msg = GetRequestFromHMI(id);
  if (!msg) {
    LOG4CXX_WARN(logger_, "DBus: request from HMI is not found");
    return;
  }

  DBusMessage* error;
  error = dbus_message_new_error(msg, name.c_str(), description.c_str());
  if (NULL == error) {
    LOG4CXX_WARN(logger_, "DBus: Failed call method (Message Null)");
    return;
  }

  dbus_uint32_t serial;
  if (!dbus_connection_send(conn_, error, &serial)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Can't send message)");
    dbus_message_unref(error);
    dbus_message_unref(msg);
    return;
  }
  dbus_connection_flush(conn_);
  dbus_message_unref(error);
  dbus_message_unref(msg);
  LOG4CXX_INFO(logger_, "DBus: Success error");
}

void DBusAdapter::MethodCall(uint id,
                             const MessageId func_id,
                             const MessageName& name,
                             const smart_objects::SmartObject& obj) {
  LOG4CXX_DEBUG(logger_, "Method call " << name.first << "." << name.second);
  if (conn_ == NULL) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }

  if (func_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return;
  }

  DBusMessage* msg;
  msg = dbus_message_new_method_call(
      hmi_service_name_.c_str(),
      hmi_object_path_.c_str(),
      (hmi_service_name_ + "." + name.first).c_str(),
      name.second.c_str());
  if (NULL == msg) {
    LOG4CXX_WARN(logger_, "DBus: Failed call method (Message Null)");
    return;
  }

  const ListArgs& args =
      schema_->getListArgs(func_id, hmi_apis::messageType::request);
  if (!SetArguments(msg, args, obj)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Signature is wrong)");
    dbus_message_unref(msg);
    return;
  }

  dbus_uint32_t serial;
  if (!dbus_connection_send(conn_, msg, &serial)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Can't send message)");
    dbus_message_unref(msg);
    return;
  }
  SaveRequestToHMI(serial, std::make_pair(id, func_id));
  dbus_message_unref(msg);
  LOG4CXX_INFO(logger_, "DBus: Success call method");
}

void DBusAdapter::Signal(const MessageId func_id,
                         const MessageName& name,
                         const smart_objects::SmartObject& obj) {
  LOG4CXX_DEBUG(logger_, "Signal " << name.first << "." << name.second);
  if (conn_ == NULL) {
    LOG4CXX_ERROR(logger_, "DBus: DBusAdaptor isn't init");
    return;
  }

  if (func_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return;
  }

  DBusMessage* msg;
  msg = dbus_message_new_signal(sdl_object_path_.c_str(),
                                (sdl_service_name_ + "." + name.first).c_str(),
                                name.second.c_str());
  if (NULL == msg) {
    LOG4CXX_WARN(logger_, "DBus: Failed emit signal (Message Null)");
    return;
  }

  const ListArgs& args =
      schema_->getListArgs(func_id, hmi_apis::messageType::notification);
  if (!SetArguments(msg, args, obj)) {
    LOG4CXX_ERROR(logger_, "DBus: Failed call method (Signature is wrong)");
    dbus_message_unref(msg);
    return;
  }

  if (!dbus_connection_send(conn_, msg, NULL)) {  // serial isn't required
    LOG4CXX_WARN(logger_, "DBus: Failed emit signal (Out Of Memory)");
    dbus_message_unref(msg);
    return;
  }
  dbus_message_unref(msg);
  LOG4CXX_INFO(logger_, "DBus: Success emit signal");
}

void DBusAdapter::AddMatch(const std::string& rule) {
  LOG4CXX_INFO(logger_, "Subscription: " << rule);
  dbus_bus_add_match(conn_, rule.c_str(), NULL);
}

bool DBusAdapter::ProcessMethodCall(DBusMessage* msg,
                                    smart_objects::SmartObject& obj) {
  std::string method = dbus_message_get_member(msg);
  std::string interface = dbus_message_get_interface(msg);
  LOG4CXX_INFO(logger_, "DBus: name of method " << interface << " " << method);

  if (interface == "org.freedesktop.DBus.Introspectable" &&
      method == "Introspect") {
    LOG4CXX_INFO(logger_, "DBus: INTROSPECT");
    Introspect(msg);
    return false;
  }

  std::vector<std::string> elems;
  split(interface, '.', elems);
  MessageName name(elems.back(), method);
  MessageId m_id = schema_->getMessageId(name);
  if (m_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method call from hmi");
    return false;
  }

  uint32_t serial = dbus_message_get_serial(msg);
  if (!serial) {
    LOG4CXX_ERROR(logger_, "DBus: Serial of request from HMI isn't defined");
    return false;
  }

  obj[sos::S_PARAMS][sos::S_CORRELATION_ID] = serial;
  obj[sos::S_PARAMS][sos::S_FUNCTION_ID] = m_id;
  obj[sos::S_PARAMS][sos::S_MESSAGE_TYPE] = hmi_apis::messageType::request;
  obj[sos::S_MSG_PARAMS] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  const ListArgs args =
      schema_->getListArgs(name, hmi_apis::messageType::request);

  DBusMessageIter iter;
  dbus_message_iter_init(msg, &iter);
  if (GetArguments(&iter, args, obj[sos::S_MSG_PARAMS])) {
    SaveRequestFromHMI(serial, msg);
    return true;
  }
  return false;
}

bool DBusAdapter::ProcessMethodReturn(DBusMessage* msg,
                                      smart_objects::SmartObject& obj) {
  LOG4CXX_AUTO_TRACE(logger_);
  dbus_uint32_t reply_serial = dbus_message_get_reply_serial(msg);
  std::pair<uint, MessageId> ids = GetRequestToHMI(reply_serial);
  if (ids.second == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return false;
  }

  obj[sos::S_MSG_PARAMS] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  ListArgs args =
      schema_->getListArgs(ids.second, hmi_apis::messageType::response);
  DBusMessageIter iter;
  dbus_message_iter_init(msg, &iter);
  int code = 0;
  std::string message;
  smart_objects::SmartObject description(smart_objects::SmartType_Map);
  bool ret = GetHeader(&iter, &code, &message) &&
             GetArguments(&iter, args, description);

  if (ret) {
    obj[sos::S_PARAMS][sos::S_CORRELATION_ID] = ids.first;
    obj[sos::S_PARAMS][sos::S_FUNCTION_ID] = ids.second;
    obj[sos::S_PARAMS][sos::S_MESSAGE_TYPE] = hmi_apis::messageType::response;
    obj[sos::S_PARAMS][sos::kCode] = code;
    obj[sos::S_PARAMS][sos::kMessage] = message;
    if (code != hmi_apis::Common_Result::SUCCESS) {
      MessageName name = schema_->getMessageName(ids.second);
      description["method"] = name.first + "." + name.second;
      obj[sos::S_PARAMS]["data"] = description;
    } else {
      obj[sos::S_MSG_PARAMS] = description;
    }
  }

  dbus_message_unref(msg);
  return ret;
}

bool DBusAdapter::ProcessError(DBusMessage* msg,
                               smart_objects::SmartObject& obj) {
  dbus_uint32_t reply_serial = dbus_message_get_reply_serial(msg);
  std::pair<uint, MessageId> ids = GetRequestToHMI(reply_serial);
  if (ids.second == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name method");
    return false;
  }

  const char* error_name;
  bool ret = false;
  if ((error_name = dbus_message_get_error_name(msg)) != NULL) {
    smart_objects::SmartObject name(smart_objects::SmartType_String);
    name = error_name;
    ford_message_descriptions::ParameterDescription rule = {
        "description", ford_message_descriptions::String, true};
    ListArgs args;
    args.push_back(&rule);
    smart_objects::SmartObject description(smart_objects::SmartType_Map);
    description[rule.name] =
        smart_objects::SmartObject(smart_objects::SmartType_String);

    DBusMessageIter iter;
    dbus_message_iter_init(msg, &iter);
    ret = GetArguments(&iter, args, description);
    MessageName method = schema_->getMessageName(ids.second);

    obj[sos::S_PARAMS][sos::S_CORRELATION_ID] = ids.first;
    obj[sos::S_PARAMS][sos::S_FUNCTION_ID] = ids.second;
    obj[sos::S_PARAMS][sos::S_MESSAGE_TYPE] =
        hmi_apis::messageType::error_response;
    obj[sos::S_PARAMS][sos::kCode] = name.asInt();
    obj[sos::S_PARAMS][sos::kMessage] = description[rule.name].asString();
    obj[sos::S_MSG_PARAMS] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    obj[sos::S_PARAMS]["data"]["method"] = method.first + "." + method.second;

    LOG4CXX_WARN(logger_,
                 "DBus: Call of method " << method.first << "." << method.second
                                         << " returned error " << name.asInt()
                                         << ": "
                                         << description[rule.name].asString());
  } else {
    LOG4CXX_ERROR(logger_, "DBus: Type message isn't error");
  }

  dbus_message_unref(msg);
  return ret;
}

bool DBusAdapter::ProcessSignal(DBusMessage* msg,
                                smart_objects::SmartObject& obj) {
  std::string method = dbus_message_get_member(msg);
  std::string interface = dbus_message_get_interface(msg);
  LOG4CXX_INFO(logger_, "DBus: name of signal " << method);

  std::vector<std::string> elems;
  split(interface, '.', elems);
  MessageName name(elems.back(), method);
  MessageId m_id = schema_->getMessageId(name);
  if (m_id == hmi_apis::FunctionID::INVALID_ENUM) {
    LOG4CXX_ERROR(logger_, "DBus: Invalid name signal");
    return false;
  }

  obj[sos::S_PARAMS][sos::S_FUNCTION_ID] = m_id;
  obj[sos::S_PARAMS][sos::S_MESSAGE_TYPE] = hmi_apis::messageType::notification;
  obj[sos::S_MSG_PARAMS] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  const ListArgs args =
      schema_->getListArgs(name, hmi_apis::messageType::notification);

  DBusMessageIter iter;
  dbus_message_iter_init(msg, &iter);
  bool ret = GetArguments(&iter, args, obj[sos::S_MSG_PARAMS]);
  dbus_message_unref(msg);
  return ret;
}

bool DBusAdapter::SetArguments(DBusMessage* msg,
                               const ListArgs& rules,
                               const smart_objects::SmartObject& args) {
  DBusMessageIter iter;
  dbus_message_iter_init_append(msg, &iter);
  size_t size = rules.size();
  for (size_t i = 0; i < size; ++i) {
    const smart_objects::SmartObject& param = args.getElement(rules[i]->name);
    if (!SetOneArgument(&iter, rules[i], param)) {
      return false;
    }
  }
  return true;
}

bool DBusAdapter::SetOneArgument(DBusMessageIter* iter,
                                 const ParameterDescription* rules,
                                 const smart_objects::SmartObject& param) {
  if (rules->obligatory) {
    if (param.isValid()) {
      return SetValue(iter, rules, param);
    } else {
      LOG4CXX_WARN(logger_,
                   "DBus: Argument '" << rules->name << "' is obligatory!");
      return false;
    }
  } else {
    return SetOptionalValue(iter, rules, param);
  }
}

bool DBusAdapter::SetValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::ParameterDescription* rules,
    const smart_objects::SmartObject& param) {
  // LOG4CXX_DEBUG(logger_, "DBus: Set param " << rules->name << " = " <<
  // param.asString());
  int type = 0;
  void* value = 0;
  dbus_int32_t integerValue = 0;
  double floatValue = 0;
  dbus_bool_t booleanValue = false;
  const char* stringValue;
  switch (rules->type) {
    case ford_message_descriptions::ParameterType::Array:
      return SetArrayValue(
          iter,
          reinterpret_cast<const ford_message_descriptions::ArrayDescription*>(
              rules),
          param);
      break;
    case ford_message_descriptions::ParameterType::Struct:
      return SetStructValue(
          iter,
          reinterpret_cast<const ford_message_descriptions::StructDescription*>(
              rules),
          param);
      break;
    case ford_message_descriptions::ParameterType::Enum:
    case ford_message_descriptions::ParameterType::Integer:
      type = DBUS_TYPE_INT32;
      integerValue = param.asInt();
      value = &integerValue;
      break;
    case ford_message_descriptions::ParameterType::Float:
      type = DBUS_TYPE_DOUBLE;
      floatValue = param.asDouble();
      value = &floatValue;
      break;
    case ford_message_descriptions::ParameterType::Boolean:
      type = DBUS_TYPE_BOOLEAN;
      booleanValue = param.asBool();
      value = &booleanValue;
      break;
    case ford_message_descriptions::ParameterType::String:
      type = DBUS_TYPE_STRING;
      stringValue = param.asString().c_str();
      value = &stringValue;
      break;
    default:
      LOG4CXX_ERROR(logger_, "DBus: Unknown type of argument");
      return false;
  }
  return dbus_message_iter_append_basic(iter, type, value);
}

bool DBusAdapter::SetOptionalValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::ParameterDescription* rules,
    const smart_objects::SmartObject& param) {
  DBusMessageIter sub_iter;
  if (!dbus_message_iter_open_container(
          iter, DBUS_TYPE_STRUCT, NULL, &sub_iter)) {
    LOG4CXX_ERROR(
        logger_,
        "DBus: Can't open container type (STRUCT) for optional parameter");
    return false;
  }

  ford_message_descriptions::ParameterDescription flagRules = {
      "flag", ford_message_descriptions::Boolean, true};
  smart_objects::SmartObject flag(param.getType() !=
                                  smart_objects::SmartType_Invalid);
  if (!SetValue(&sub_iter, &flagRules, flag) ||
      !SetValue(&sub_iter, rules, param)) {
    return false;
  }

  if (!dbus_message_iter_close_container(iter, &sub_iter)) {
    LOG4CXX_ERROR(
        logger_,
        "DBus: Can't close container type (STRUCT) for optional parameter");
    return false;
  }
  return true;
}

bool DBusAdapter::SetArrayValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::ArrayDescription* rules,
    const smart_objects::SmartObject& param) {
  smart_objects::SmartType type = param.getType();
  if (type != smart_objects::SmartType_Array &&
      type != smart_objects::SmartType_Invalid) {
    LOG4CXX_ERROR(logger_, "DBus: SmartObject is not a map");
    return false;
  }
  DBusMessageIter sub_iter;
  if (!dbus_message_iter_open_container(
          iter, DBUS_TYPE_ARRAY, rules->element_dbus_signature, &sub_iter)) {
    LOG4CXX_ERROR(logger_, "DBus: Can't open container type (ARRAY)");
    return false;
  }
  size_t size = param.length();
  for (size_t i = 0; i < size; ++i) {
    if (!SetValue(&sub_iter, rules->element, param[i])) {
      return false;
    }
  }
  if (!dbus_message_iter_close_container(iter, &sub_iter)) {
    LOG4CXX_ERROR(logger_, "DBus: Can't close container type (ARRAY)");
    return false;
  }
  return true;
}

bool DBusAdapter::SetStructValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::StructDescription* rules,
    const smart_objects::SmartObject& structure) {
  smart_objects::SmartType type = structure.getType();
  if (type != smart_objects::SmartType_Map &&
      type != smart_objects::SmartType_Invalid) {
    LOG4CXX_ERROR(logger_, "DBus: SmartObject is not a map");
    return false;
  }
  DBusMessageIter sub_iter;
  if (!dbus_message_iter_open_container(
          iter, DBUS_TYPE_STRUCT, NULL, &sub_iter)) {
    LOG4CXX_ERROR(logger_, "DBus: Can't open container type (STRUCT)");
    return false;
  }
  const ParameterDescription** entry;
  entry = rules->parameters;
  while (*entry != NULL) {
    const smart_objects::SmartObject& param =
        structure.getElement((*entry)->name);
    if (!SetOneArgument(&sub_iter, *entry, param)) {
      return false;
    }
    entry++;
  }
  if (!dbus_message_iter_close_container(iter, &sub_iter)) {
    LOG4CXX_ERROR(logger_, "DBus: Can't close container type (STRUCT)");
    return false;
  }
  return true;
}

bool DBusAdapter::GetHeader(DBusMessageIter* iter,
                            int* code,
                            std::string* message) {
  // Get code of response
  dbus_int32_t intValue;
  int type = dbus_message_iter_get_arg_type(iter);
  if (type != DBUS_TYPE_INT32) {
    LOG4CXX_ERROR(logger_, "DBus: Unknown format of header");
    return false;
  }
  dbus_message_iter_get_basic(iter, &intValue);
  *code = intValue;
  LOG4CXX_DEBUG(logger_, "DBus: Code of response " << *code);

  dbus_message_iter_next(iter);

  // Get message of response
  type = dbus_message_iter_get_arg_type(iter);
  if (type != DBUS_TYPE_STRING) {
    LOG4CXX_ERROR(logger_, "DBus: Unknown format of header");
    return false;
  }
  const char* stringValue;
  dbus_message_iter_get_basic(iter, &stringValue);
  *message = stringValue;
  LOG4CXX_DEBUG(logger_, "DBus: message of response " << *message);

  dbus_message_iter_next(iter);
  return true;
}

bool DBusAdapter::GetArguments(DBusMessageIter* iter,
                               const ListArgs& rules,
                               smart_objects::SmartObject& args) {
  LOG4CXX_AUTO_TRACE(logger_);

  size_t size = rules.size();
  for (size_t i = 0; i < size; ++i) {
    if (!GetOneArgument(iter, rules[i], args)) {
      return false;
    }
    dbus_message_iter_next(iter);
  }
  return true;
}

const DBusSchema& DBusAdapter::get_schema() const {
  return *schema_;
}

bool DBusAdapter::GetOneArgument(
    DBusMessageIter* iter,
    const ford_message_descriptions::ParameterDescription* rules,
    smart_objects::SmartObject& args) {
  if (rules->obligatory) {
    return GetValue(iter, rules, args[rules->name]);
  } else {
    return GetOptionalValue(iter, rules, args);
  }
}

bool DBusAdapter::GetValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::ParameterDescription* rules,
    smart_objects::SmartObject& param) {
  LOG4CXX_DEBUG(logger_, "DBus: Get param " << rules->name);
  int type = dbus_message_iter_get_arg_type(iter);
  switch (rules->type) {
    case ford_message_descriptions::ParameterType::Array:
      if (type == DBUS_TYPE_ARRAY) {
        return GetArrayValue(
            iter,
            // FIXME (dchmerev@luxoft.com): not portable, danger cast.
            reinterpret_cast<
                const ford_message_descriptions::ArrayDescription*>(rules),
            param);
      } else {
        LOG4CXX_ERROR(logger_, "DBus: Not expected type of argument");
        return false;
      }
      break;
    case ford_message_descriptions::ParameterType::Struct:
      if (type == DBUS_TYPE_STRUCT) {
        return GetStructValue(
            iter,
            reinterpret_cast<
                const ford_message_descriptions::StructDescription*>(rules),
            param);
      } else {
        LOG4CXX_ERROR(logger_, "DBus: Not expected type of argument");
        return false;
      }
      break;
    case ford_message_descriptions::ParameterType::Enum:
    case ford_message_descriptions::ParameterType::Integer:
      if (type == DBUS_TYPE_INT32) {
        dbus_int32_t integerValue;
        dbus_message_iter_get_basic(iter, &integerValue);
        smart_objects::SmartObject value(integerValue);
        param = value;
        LOG4CXX_DEBUG(logger_,
                      "DBus: " << rules->name << " = " << integerValue);
      } else {
        LOG4CXX_ERROR(logger_, "DBus: Not expected type of argument");
        return false;
      }
      break;
    case ford_message_descriptions::ParameterType::Float:
      if (type == DBUS_TYPE_DOUBLE) {
        double floatValue;
        dbus_message_iter_get_basic(iter, &floatValue);
        smart_objects::SmartObject value(floatValue);
        param = value;
        LOG4CXX_DEBUG(logger_, "DBus: " << rules->name << " = " << floatValue);
      } else {
        LOG4CXX_ERROR(logger_, "DBus: Not expected type of argument");
        return false;
      }
      break;
    case ford_message_descriptions::ParameterType::Boolean:
      if (type == DBUS_TYPE_BOOLEAN) {
        dbus_bool_t booleanValue;
        dbus_message_iter_get_basic(iter, &booleanValue);
        smart_objects::SmartObject value(static_cast<bool>(booleanValue));
        param = value;
        LOG4CXX_DEBUG(logger_,
                      "DBus: " << rules->name << " = " << std::boolalpha
                               << booleanValue);
      } else {
        LOG4CXX_ERROR(logger_, "DBus: Not expected type of argument");
        return false;
      }
      break;
    case ford_message_descriptions::ParameterType::String:
      if (type == DBUS_TYPE_STRING) {
        const char* stringValue;
        dbus_message_iter_get_basic(iter, &stringValue);
        std::string strValue = stringValue;
        smart_objects::SmartObject value(strValue);
        param = value;
        LOG4CXX_DEBUG(logger_,
                      "DBus: " << rules->name << " = \"" << strValue << "\"");
      } else {
        LOG4CXX_ERROR(logger_, "DBus: Not expected type of argument");
        return false;
      }
      break;
    default:
      LOG4CXX_ERROR(logger_, "DBus: Unknown type of argument");
      return false;
  }
  return true;
}

bool DBusAdapter::GetArrayValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::ArrayDescription* rules,
    smart_objects::SmartObject& param) {
  smart_objects::SmartObject array(smart_objects::SmartType_Array);

  int i = 0;
  DBusMessageIter sub_iter;
  dbus_message_iter_recurse(iter, &sub_iter);
  while (dbus_message_iter_get_arg_type(&sub_iter) != DBUS_TYPE_INVALID) {
    if (!GetValue(&sub_iter, rules->element, array[i])) {
      return false;
    }
    dbus_message_iter_next(&sub_iter);
    i++;
  }
  param = array;
  return true;
}

bool DBusAdapter::GetStructValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::StructDescription* rules,
    smart_objects::SmartObject& param) {
  DBusMessageIter sub_iter;
  dbus_message_iter_recurse(iter, &sub_iter);
  const ParameterDescription** entry;
  entry = rules->parameters;
  smart_objects::SmartObject structure(smart_objects::SmartType_Map);
  while (*entry != NULL) {
    if (!GetOneArgument(&sub_iter, *entry, structure)) {
      return false;
    }
    dbus_message_iter_next(&sub_iter);
    entry++;
  }
  param = structure;
  return true;
}

bool DBusAdapter::GetOptionalValue(
    DBusMessageIter* iter,
    const ford_message_descriptions::ParameterDescription* rules,
    smart_objects::SmartObject& param) {
  int type = dbus_message_iter_get_arg_type(iter);
  if (type != DBUS_TYPE_STRUCT) {
    LOG4CXX_WARN(
        logger_,
        "DBus: Not expected type of argument. It is not optional parameter.");
    return false;
  }

  DBusMessageIter sub_iter;
  dbus_message_iter_recurse(iter, &sub_iter);
  ford_message_descriptions::ParameterDescription flagRules = {
      "flag", ford_message_descriptions::Boolean, true};
  smart_objects::SmartObject flag;
  if (!GetValue(&sub_iter, &flagRules, flag)) {
    return false;
  }
  if (flag.asBool()) {
    dbus_message_iter_next(&sub_iter);
    return GetValue(&sub_iter, rules, param[rules->name]);
  } else {
    return true;
  }
}

std::pair<uint, MessageId> DBusAdapter::GetRequestToHMI(uint32_t serial) {
  std::map<uint32_t, std::pair<uint, MessageId> >::iterator it;
  it = requests_to_hmi_.find(serial);
  if (it != requests_to_hmi_.end()) {
    std::pair<uint, MessageId> ids = it->second;
    requests_to_hmi_.erase(it);
    return ids;
  }
  return std::make_pair(0, hmi_apis::FunctionID::INVALID_ENUM);
}

void DBusAdapter::SaveRequestToHMI(uint32_t serial,
                                   const std::pair<uint, MessageId>& ids) {
  requests_to_hmi_.insert(std::make_pair(serial, ids));
}

void DBusAdapter::Introspect(DBusMessage* msg) {
  DBusMessage* reply;

  dbus_uint32_t serial = dbus_message_get_serial(msg);
  reply = dbus_message_new_method_return(msg);
  if (!reply) {
    LOG4CXX_WARN(logger_, "DBus: Failed return method for introspection");
    return;
  }

  DBusMessageIter iter;
  dbus_message_iter_init_append(reply, &iter);
  char* value = introspection_xml;
  dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &value);
  dbus_connection_send(conn_, reply, &serial);
  dbus_connection_flush(conn_);
  dbus_message_unref(reply);
  dbus_message_unref(msg);
}

void DBusAdapter::SaveRequestFromHMI(uint32_t serial, DBusMessage* request) {
  LOG4CXX_DEBUG(logger_, "Save correlation id (serial id) : " << serial);
  LOG4CXX_DEBUG(logger_, "D-Bus message: " << request);
  requests_from_hmi_.insert(std::make_pair(serial, request));
}

DBusMessage* DBusAdapter::GetRequestFromHMI(uint32_t serial) {
  LOG4CXX_DEBUG(logger_, "Get correlation id (serial id) : " << serial);
  std::map<uint32_t, DBusMessage*>::iterator it;
  it = requests_from_hmi_.find(serial);
  if (it != requests_from_hmi_.end()) {
    DBusMessage* msg = it->second;
    requests_from_hmi_.erase(it);
    LOG4CXX_DEBUG(logger_, "D-Bus message: " << msg);
    return msg;
  }
  return 0;
}

}  // namespace dbus
