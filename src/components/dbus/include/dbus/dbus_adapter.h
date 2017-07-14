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
#ifndef SRC_COMPONENTS_DBUS_INCLUDE_DBUS_DBUS_ADAPTER_H_
#define SRC_COMPONENTS_DBUS_INCLUDE_DBUS_DBUS_ADAPTER_H_

#include <string>
#include "smart_objects/smart_object.h"
#include "dbus/schema.h"

struct DBusConnection;
struct DBusMessage;
struct DBusMessageIter;

namespace dbus {

/**
 * \brief class for work with DBus
 */
class DBusAdapter {
 public:
  /**
   * \brief constructs DBus adapter
   * \param sdlServiceName core service name
   * \param sdlObjectPath core object path
   * \param hmiServiceName hmi service name
   */
  DBusAdapter(const std::string& sdlServiceName,
              const std::string& sdlObjectPath,
              const std::string& hmiServiceName,
              const std::string& hmiObjectPath);

  /**
   * \brief destructs DBus adapter
   */
  virtual ~DBusAdapter();

  /**
   * \brief inits service
   * \return true if success
   */
  bool Init();

  /**
   * \brief return schema messages for DBus
   * \return schema
   */
  const DBusSchema& get_schema() const;

 protected:
  /**
   * \brief calls method on HMI
   * \param id id message
   * \param func_id id function in Ford protocol
   * \param name pair interface and name of method for call
   * \param obj params for call
   */
  void MethodCall(uint id,
                  const MessageId func_id,
                  const MessageName& name,
                  const smart_objects::SmartObject& obj);

  /**
   * \brief sends signal
   * \param id id message
   * \param func_id id function in Ford protocol
   * \param name pair interface and name of signal for call
   * \param obj params for signal
   */
  void Signal(const MessageId func_id,
              const MessageName& name,
              const smart_objects::SmartObject& obj);

  /**
   * \brief returns result of call method to HMI
   * \param id id message
   * \param obj params for return
   */
  void MethodReturn(uint id,
                    const MessageId func_id,
                    const MessageName& name,
                    const smart_objects::SmartObject& obj);

  /**
   * \brief sends error on message from HMI
   * \param id id message
   * \param msg message from HMI
   * \param name name of error
   * \param description description of error
   */
  void Error(uint id, const std::string& name, const std::string& description);

  /**
   * \brief adds a match rule
   * \param rule string rule in the DBus specification
   */
  void AddMatch(const std::string& rule);

  /**
   * \brief processes incoming message from DBus if queue isn't empty
   * and fill obj
   * \param obj object for send to core
   * \return true if message processed
   */
  bool Process(smart_objects::SmartObject& obj);

  /**
   * \brief saves link D-Bus serial to Ford message id
   * \param serial D-Bus message serial
   * \param ids pair correlation id and Ford message id
   */
  inline void SaveRequestToHMI(uint32_t serial,
                               const std::pair<uint, MessageId>& ids);

  /**
   * \brief gets Ford message id by serial
   * \param serial D-Bus message serial
   * \return pair correlation id and Ford message id
   */
  inline std::pair<uint, MessageId> GetRequestToHMI(uint32_t serial);

  /**
   * \brief saves link D-Bus serial to Ford message id
   * \param serial D-Bus message serial
   * \param request D-Bus message from HMI
   */
  inline void SaveRequestFromHMI(uint32_t serial, DBusMessage* request);

  /**
   * \brief gets D-Bus message id by serial
   * \param serial DBus message serial
   * \return D-Bus message from HMI
   */
  inline DBusMessage* GetRequestFromHMI(uint32_t serial);

  std::string sdl_service_name_;
  std::string sdl_object_path_;
  std::string hmi_service_name_;
  std::string hmi_object_path_;
  DBusConnection* conn_;

 private:
  /**
   * \brief schema messages and arguments for DBus
   */
  const DBusSchema* schema_;

  /**
   * \brief mapping serial message DBus on message id Ford protocol
   */
  std::map<uint32_t, std::pair<uint, MessageId> > requests_to_hmi_;

  /**
   * \brief mapping message id Ford protocol on message DBus
   */
  std::map<uint32_t, DBusMessage*> requests_from_hmi_;

  /**
   * \brief processes incoming call of method and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   * \return true if success
   */
  bool ProcessMethodCall(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief processes incoming return of method and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   * \return true if success
   */
  bool ProcessMethodReturn(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief processes incoming error and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   * \return true if success
   */
  bool ProcessError(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief processes incoming signal and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   * \return true if success
   */
  bool ProcessSignal(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief sets arguments to message
   * \param msg DBus message
   * \param rules list of rules for arguments
   * \param args map of arguments
   * \return true if success
   */
  bool SetArguments(DBusMessage* msg,
                    const ListArgs& rules,
                    const smart_objects::SmartObject& args);

  /**
   * \brief Sets one argument to message
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param value of argument
   * \return true if success
   */
  bool SetOneArgument(
      DBusMessageIter* iter,
      const ford_message_descriptions::ParameterDescription* rules,
      const smart_objects::SmartObject& param);

  /**
   * \brief sets value for argument
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param value of argument
   * \return true if success
   */
  bool SetValue(DBusMessageIter* iter,
                const ford_message_descriptions::ParameterDescription* rules,
                const smart_objects::SmartObject& param);

  /**
   * \brief sets value for every element of argument
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param array values of argument
   * \return true if success
   */
  bool SetArrayValue(DBusMessageIter* iter,
                     const ford_message_descriptions::ArrayDescription* rules,
                     const smart_objects::SmartObject& param);

  /**
   * \brief sets struct value for argument
   * \param iter DBus message iter
   * \param rules description for argument
   * \param param structure
   * \return true if success
   */
  bool SetStructValue(DBusMessageIter* iter,
                      const ford_message_descriptions::StructDescription* rules,
                      const smart_objects::SmartObject& param);

  /**
   * \brief sets optional value for argument.
   * Optional param is struct bool, value
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param value of optional argument
   * \return true if success
   */
  bool SetOptionalValue(
      DBusMessageIter* iter,
      const ford_message_descriptions::ParameterDescription* rules,
      const smart_objects::SmartObject& param);

  /**
   * \brief gets arguments from message
   * \param iter DBus message iterator
   * \param code response code (output)
   * \param message response message (output)
   * \return true if success
   */
  bool GetHeader(DBusMessageIter* iter, int* code, std::string* message);

  /**
   * \brief gets arguments from message with header
   * \param iter DBus message iterator
   * \param rules list of rules for arguments
   * \param args map of arguments
   * \return true if success
   */
  bool GetArguments(DBusMessageIter* iter,
                    const ListArgs& rules,
                    smart_objects::SmartObject& args);

  /**
   * \brief gets one argument from message
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param args map of arguments
   * \return true if success
   */
  bool GetOneArgument(
      DBusMessageIter* iter,
      const ford_message_descriptions::ParameterDescription* rules,
      smart_objects::SmartObject& args);

  /**
   * \brief gets value for argument
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param value of argument
   * \return true if success
   */
  bool GetValue(DBusMessageIter* iter,
                const ford_message_descriptions::ParameterDescription* rules,
                smart_objects::SmartObject& param);

  /**
   * \brief gets value for every element of argument
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param array values of argument
   * \return true if success
   */
  bool GetArrayValue(DBusMessageIter* iter,
                     const ford_message_descriptions::ArrayDescription* rules,
                     smart_objects::SmartObject& param);

  /**
   * \brief gets struct value for argument
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param structure
   * \return true if success
   */
  bool GetStructValue(DBusMessageIter* iter,
                      const ford_message_descriptions::StructDescription* rules,
                      smart_objects::SmartObject& param);

  /**
   * \brief gets optional value for argument.
   * Optional param is struct bool, value
   * \param iter DBus message iterator
   * \param rules description for argument
   * \param param value of optional argument
   * \return true if success
   */
  bool GetOptionalValue(
      DBusMessageIter* iter,
      const ford_message_descriptions::ParameterDescription* rules,
      smart_objects::SmartObject& param);

  /**
   * \brief processes request on introspect
   * \param msg DBus message
   */
  void Introspect(DBusMessage* msg);
};

}  // namespace dbus

#endif  // SRC_COMPONENTS_DBUS_INCLUDE_DBUS_DBUS_ADAPTER_H_
