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
#ifndef SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_ADAPTER_H_
#define SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_ADAPTER_H_

#include <string>
#include "utils/logger.h"
#include "smart_objects/smart_object.h"
#include "dbus_schema/schema.h"

struct DBusConnection;
struct DBusMessage;
struct DBusMessageIter;

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

namespace hmi_message_handler {

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
  const dbus_schema::DBusSchema& get_schema() const;

 protected:
  static log4cxx::LoggerPtr logger_;

  /**
   * \brief calls method on HMI
   * \param id id message
   * \param path name of interface on HMI
   * \param method name of method for call
   * \param obj params for call
   */
  void MethodCall(uint id, const std::string& path, const std::string& method,
                  smart_objects::SmartObject& obj);

  /**
   * \brief sends signal
   * \param id id message
   * \param interface name of interface
   * \param signal name of signal for send
   * \param obj params for signal
   */
  void Signal(uint id, const std::string& interface, const std::string& signal,
              smart_objects::SmartObject& obj);

  /**
   * \brief returns result of call method to HMI
   * \param id id message
   * \param obj params for return
   */
  void MethodReturn(uint id, smart_objects::SmartObject& obj);

  /**
   * \brief sends error on message from HMI
   * \param id id message
   * \param msg message from HMI
   * \param name name of error
   * \param description description of error
   */
  void Error(uint id, const std::string& name,
             const std::string& description);

  /**
   * \brief adds a match rule
   * \param rule string rule in the DBus specification
   * \return false if error
   */
  bool AddMatch(const std::string& rule);

  /**
   * \brief processes message from DBus if queue isn't empty
   * and fill obj
   * \param obj object for send to core
   * \return true if message processed
   */
  bool Process(smart_objects::SmartObject& obj);

  std::string sdl_service_name_;
  std::string sdl_object_path_;
  std::string hmi_service_name_;
  std::string hmi_object_path_;
  DBusConnection* conn_;

  void Reply(DBusMessage* msg, DBusConnection* conn, std::string& message);

 private:
  /**
   * \brief schema messages and arguments for DBus
   */
  dbus_schema::DBusSchema schema_;

  /**
   * \brief processes call of method and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   */
  void ProcessMethodCall(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief processes return of method and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   */
  void ProcessMethodReturn(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief processes error and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   */
  void ProcessError(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief processes signal and fill obj
   * \param msg message from DBus
   * \param obj object for send to core
   */
  void ProcessSignal(DBusMessage* msg, smart_objects::SmartObject& obj);

  /**
   * \brief sets arguments to message
   * \param msg DBus message
   * \param rules list of rules for arguments
   * \param obj map of arguments
   * \return true if success
   */
  bool SetArgs(DBusMessage* msg, const dbus_schema::ListArgs& rules,
               smart_objects::SmartObject& obj);

  /**
   * \brief gets arguments from message
   * @param msg DBus message
   * @return true if success
   */
  bool GetArgs(DBusMessage* msg);

//  bool SetBasicArg(DBusMessageIter* args,
//                   dbus_schema::ParameterDescription* rule,
//                   smart_objects::SmartObject& obj);
//
//  bool SetArrayArg(DBusMessageIter* args,
//                   dbus_schema::ParameterDescription* rule,
//                   smart_objects::SmartObject& obj);
};

}  // namespace hmi_message_handler

#endif  // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_ADAPTER_H_
