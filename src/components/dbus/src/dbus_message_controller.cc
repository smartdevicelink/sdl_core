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

#include "dbus/dbus_message_controller.h"
#include "formatters/CSmartFactory.hpp"

namespace sos = NsSmartDeviceLink::NsJSONHandler::strings;

namespace dbus {

DBusMessageController::DBusMessageController(const std::string& sdlServiceName,
                                             const std::string& sdlObjectPath,
                                             const std::string& hmiServiceName,
                                             const std::string& hmiObjectPath)
    : DBusAdapter(sdlServiceName, sdlObjectPath,
                  hmiServiceName, hmiObjectPath) {}

void DBusMessageController::SubscribeTo(const std::string& interface,
                                        const std::string& signal) {
  std::string rule = "type='signal'";
  rule.append(", sender='").append(hmi_service_name_).append("'")
      .append(", path='").append(hmi_object_path_).append("'")
      .append(", interface='").append(hmi_service_name_).append(".")
      .append(interface).append("'")
      .append(", member='").append(signal).append("'");
  DBusAdapter::AddMatch(rule);
}

DBusMessageController::~DBusMessageController() {
}

void* DBusMessageController::MethodForReceiverThread(void*) {
  while (true) {
    smart_objects::SmartObject obj(smart_objects::SmartType_Map);
    obj[sos::S_PARAMS][sos::S_PROTOCOL_VERSION] = 2;
    obj[sos::S_PARAMS][sos::S_PROTOCOL_TYPE] = 1;
    if (DBusAdapter::Process(obj)) {
      SendMessageToCore(obj);
    }
  }
  return NULL;
}

}  // namespace dbus
