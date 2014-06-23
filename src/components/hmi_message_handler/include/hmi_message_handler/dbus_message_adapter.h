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

#ifndef SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_MESSAGE_ADAPTER_H_
#define SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_MESSAGE_ADAPTER_H_

#include <string>
#include "hmi_message_handler/hmi_message_adapter.h"
#include "dbus/dbus_message_controller.h"
#include "smart_objects/smart_object.h"

namespace hmi_message_handler {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

/**
 * \brief adapter for DBus
 */
class DBusMessageAdapter
    : public HMIMessageAdapter,
      public dbus::DBusMessageController {
 public:
  typedef utils::SharedPtr<application_manager::Message> MessageSharedPointer;
  explicit DBusMessageAdapter(HMIMessageHandler* hmi_msg_handler);
  ~DBusMessageAdapter();
  void SendMessageToHMI(MessageSharedPointer message);

  /**
   * \brief subscribes to signals
   */
  void SubscribeTo();

 private:
  static const std::string SDL_SERVICE_NAME;
  static const std::string SDL_OBJECT_PATH;
  static const std::string HMI_SERVICE_NAME;
  static const std::string HMI_OBJECT_PATH;

  /**
   * \brief sends request to HMI
   * \param obj request
   */
  void Request(const smart_objects::SmartObject &obj);

  /**
   * \brief sends notification to HMI
   * \param obj notification
   */
  void Notification(const smart_objects::SmartObject& obj);

  /**
   * \brief sends response to HMI
   * \param obj response
   */
  void Response(const smart_objects::SmartObject &obj);

  /**
   * \brief sends error response to HMI
   * \param obj error
   */
  void ErrorResponse(const smart_objects::SmartObject& obj);

  /**
   * \brief sends message to core
   * \param obj
   */
  void SendMessageToCore(const smart_objects::SmartObject &obj);
};

}  // namespace hmi_message_handler

#endif  // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_DBUS_MESSAGE_ADAPTER_H_
