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

#include "hmi_message_handler/dbus_message_adapter.h"

#include <string>

#include "hmi_message_handler/hmi_message_adapter.h"
#include "hmi_message_handler/dbus_message_controller.h"

namespace hmi_message_handler {

const std::string DBusMessageAdapter::SERVICE_NAME = "sdl.core.api";
const std::string DBusMessageAdapter::PATH = "/dbus";

DBusMessageAdapter::DBusMessageAdapter(
    HMIMessageHandler* handler)
    : HMIMessageAdapter(handler),
      DBusMessageController(SERVICE_NAME, PATH) {
  LOG4CXX_INFO(logger_, "Created MessageBrokerAdapter");
}

DBusMessageAdapter::~DBusMessageAdapter() {
}

void DBusMessageAdapter::sendMessageToHMI(
    utils::SharedPtr<application_manager::Message> message) {
  LOG4CXX_INFO(logger_, "MessageBrokerAdapter::sendMessageToHMI");

  Json::Reader reader;
  Json::Value json_value;
  if (!reader.parse(message->json_message(), json_value, false)) {
    LOG4CXX_ERROR(logger_, "Received invalid json string");
    return;
  }

  SendJsonMessage(json_value);
}

void DBusMessageAdapter::subscribeTo() {
  LOG4CXX_INFO(logger_, "DBusMessageAdapter::subscribeTo");
  DBusMessageController::subscribeTo("Buttons.OnButtonEvent");
  DBusMessageController::subscribeTo("Buttons.OnButtonPress");
  DBusMessageController::subscribeTo("UI.OnCommand");
  DBusMessageController::subscribeTo("VR.OnCommand");
  DBusMessageController::subscribeTo("BasicCommunication.OnReady");
  DBusMessageController::subscribeTo("UI.OnDriverDistraction");
  DBusMessageController::subscribeTo("UI.OnSystemContext");
  DBusMessageController::subscribeTo("UI.OnAppActivated");
  DBusMessageController::subscribeTo("BasicCommunication.OnAppDeactivated");
  DBusMessageController::subscribeTo("BasicCommunication.OnStartDeviceDiscovery");
  DBusMessageController::subscribeTo("BasicCommunication.OnUpdateDeviceList");
  DBusMessageController::subscribeTo("BasicCommunication.OnFindApplications");
  DBusMessageController::subscribeTo("BasicCommunication.OnAppActivated");
  DBusMessageController::subscribeTo("BasicCommunication.OnExitApplication");
  DBusMessageController::subscribeTo("BasicCommunication.OnExitAllApplications");
  DBusMessageController::subscribeTo("BasicCommunication.OnDeviceChosen");
  DBusMessageController::subscribeTo("UI.OnLanguageChange");
  DBusMessageController::subscribeTo("VR.OnLanguageChange");
  DBusMessageController::subscribeTo("TTS.OnLanguageChange");
  DBusMessageController::subscribeTo("VehicleInfo.OnVehicleData");
  DBusMessageController::subscribeTo("UI.OnTBTClientState");
  LOG4CXX_INFO(logger_, "Subscribed to notifications.");
}

void DBusMessageAdapter::processResponse(
  std::string method, Json::Value& root) {
  LOG4CXX_INFO(logger_, "MessageBrokerAdapter::processResponse");
  processRecievedfromMB(root);
}

void DBusMessageAdapter::processRequest(Json::Value& root) {
  LOG4CXX_INFO(logger_, "MessageBrokerAdapter::processRequest");
  processRecievedfromMB(root);
}

void DBusMessageAdapter::processNotification(Json::Value& root) {
  LOG4CXX_INFO(logger_, "MessageBrokerAdapter::processNotification");
  processRecievedfromMB(root);
}

void DBusMessageAdapter::processRecievedfromMB(Json::Value& root) {
  LOG4CXX_INFO(logger_, "MessageBrokerAdapter::processRecievedfromMB");
  if (root.isNull()) {
    // LOG
    return;
  }

  Json::FastWriter writer;
  std::string message_string = writer.write(root);

  if (message_string.empty()) {
    // LOG
    return;
  }

  if (!handler()) {
    // WARNING
    return;
  }

  application_manager::Message* message =
    new application_manager::Message;
  // message->set_message_type()
  message->set_json_message(message_string);
  message->set_protocol_version(
    application_manager::ProtocolVersion::kHMI);

  handler()->onMessageReceived(message);
  LOG4CXX_INFO(logger_, "Successfully sent to observer");
}

}  // namespace hmi_message_handler
