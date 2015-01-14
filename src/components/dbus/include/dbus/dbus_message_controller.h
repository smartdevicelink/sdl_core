/*
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

#ifndef SRC_COMPONENTS_DBUS_INCLUDE_DBUS_DBUS_MESSAGE_CONTROLLER_H_
#define SRC_COMPONENTS_DBUS_INCLUDE_DBUS_DBUS_MESSAGE_CONTROLLER_H_

#include <string>
#include <map>
#include "dbus/dbus_adapter.h"

namespace dbus {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

class DBusMessageController : public DBusAdapter {
 public:
  /**
   * \brief constructs DBus message controller
   * \param sdlServiceName name of service SDL
   * \param sdlObjectPath path of object SDL
   * \param hmiServiceName name of service HMI
   * \param hmiObjectPath path of object HMI
   */
  DBusMessageController(const std::string& sdlServiceName,
                        const std::string& sdlObjectPath,
                        const std::string& hmiServiceName,
                        const std::string& hmiObjectPath);

  /**
   * \brief destructs DBus message controller
   */
  virtual ~DBusMessageController();

  /**
   * \brief subscribes to the DBus signal.
   * \param interface name of interface in HMI
   * \param signal name of signal
   */
  void SubscribeTo(const std::string& interface, const std::string& signal);

  /**
   * \brief Method for receiving thread.
   */
  void* MethodForReceiverThread(void*);

 protected:
  /**
   * \brief sends message to core
   * \param obj
   */
  virtual void SendMessageToCore(const smart_objects::SmartObject& obj) = 0;
};

}  // namespace dbus

#endif  // SRC_COMPONENTS_DBUS_INCLUDE_DBUS_DBUS_MESSAGE_CONTROLLER_H_
