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

#include "hmi_message_handler/dbus_message_adapter.h"
#include <sstream>
#include "utils/logger.h"
#include "formatters/CSmartFactory.h"

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace sos = NsSmartDeviceLink::NsJSONHandler::strings;

namespace hmi_message_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "HMIMessageHandler")

const std::string DBusMessageAdapter::SDL_SERVICE_NAME = "com.ford.sdl.core";
const std::string DBusMessageAdapter::SDL_OBJECT_PATH  = "/";
const std::string DBusMessageAdapter::HMI_SERVICE_NAME = "com.ford.sdl.hmi";
const std::string DBusMessageAdapter::HMI_OBJECT_PATH  = "/";

std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

DBusMessageAdapter::DBusMessageAdapter(HMIMessageHandler* hmi_msg_handler)
    : HMIMessageAdapterImpl(hmi_msg_handler),
      DBusMessageController(SDL_SERVICE_NAME, SDL_OBJECT_PATH,
                            HMI_SERVICE_NAME, HMI_OBJECT_PATH) {
  LOG4CXX_INFO(logger_, "Created DBusMessageAdapter");
}

DBusMessageAdapter::~DBusMessageAdapter() {
}

void DBusMessageAdapter::SendMessageToHMI(MessageSharedPointer message) {
  LOG4CXX_INFO(logger_, "DBusMessageAdapter::sendMessageToHMI");

  const smart_objects::SmartObject& smart = message->smart_object();
  switch (smart[sos::S_PARAMS][sos::S_MESSAGE_TYPE].asInt()) {
    case hmi_apis::messageType::request:
      Request(smart);
      break;
    case hmi_apis::messageType::notification:
      Notification(smart);
      break;
    case hmi_apis::messageType::response:
      Response(smart);
      break;
    case hmi_apis::messageType::error_response:
      ErrorResponse(smart);
      break;
    case hmi_apis::messageType::INVALID_ENUM:
    default:
      LOG4CXX_WARN(logger_, "Message type is invalid");
  }
}

void DBusMessageAdapter::SubscribeTo() {
  LOG4CXX_INFO(logger_, "DBusMessageAdapter::subscribeTo");
  DBusMessageController::SubscribeTo("Buttons", "OnButtonEvent");
  DBusMessageController::SubscribeTo("Buttons", "OnButtonPress");
  DBusMessageController::SubscribeTo("UI", "OnCommand");
  DBusMessageController::SubscribeTo("UI", "OnDriverDistraction");
  DBusMessageController::SubscribeTo("UI", "OnSystemContext");
  DBusMessageController::SubscribeTo("UI", "OnLanguageChange");
  DBusMessageController::SubscribeTo("UI", "OnKeyboardInput");
  DBusMessageController::SubscribeTo("UI", "OnTouchEvent");
  DBusMessageController::SubscribeTo("UI", "OnResetTimeout");
  DBusMessageController::SubscribeTo("TTS", "OnResetTimeout");
  DBusMessageController::SubscribeTo("VR", "Started");
  DBusMessageController::SubscribeTo("VR", "Stopped");
  DBusMessageController::SubscribeTo("VR", "OnCommand");
  DBusMessageController::SubscribeTo("VR", "OnLanguageChange");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnReady");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnAppDeactivated");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnStartDeviceDiscovery");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnUpdateDeviceList");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnFindApplications");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnAppActivated");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnExitApplication");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnExitAllApplications");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnDeviceChosen");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnIgnitionCycleOver");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnSystemRequest");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnSystemInfoChanged");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnPhoneCall");
  DBusMessageController::SubscribeTo("BasicCommunication", "OnEmergencyEvent");
  DBusMessageController::SubscribeTo("TTS", "Started");
  DBusMessageController::SubscribeTo("TTS", "Stopped");
  DBusMessageController::SubscribeTo("TTS", "OnLanguageChange");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnGpsData");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnSpeed");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnRpm");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnFuelLevel");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnFuelLevelState");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnInstantFuelConsumption");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnExternalTemperature");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnVin");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnPrndl");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnTirePressure");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnOdometer");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnBeltStatus");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnBodyInformation");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnDeviceStatus");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnDriverBraking");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnWiperStatus");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnHeadLampStatus");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnEngineTorque");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnAccPedalPosition");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnSteeringWheelAngle");
  DBusMessageController::SubscribeTo("VehicleInfo", "OnMyKey");
  DBusMessageController::SubscribeTo("Navigation",  "OnTBTClientState");
  DBusMessageController::SubscribeTo("SDL", "OnAllowSDLFunctionality");
  DBusMessageController::SubscribeTo("SDL", "OnReceivedPolicyUpdate");
  DBusMessageController::SubscribeTo("SDL", "OnPolicyUpdate");
  DBusMessageController::SubscribeTo("SDL", "OnAppPermissionConsent");
  DBusMessageController::SubscribeTo("SDL", "OnSystemError");
  DBusMessageController::SubscribeTo("SDL", "AddStatisticsInfo");
  DBusMessageController::SubscribeTo("SDL", "OnDeviceStateChanged");

  LOG4CXX_INFO(logger_, "Subscribed to notifications.");
}

void DBusMessageAdapter::SendMessageToCore(const smart_objects::SmartObject& obj) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!handler()) {
    LOG4CXX_WARN(logger_, "DBusMessageAdapter hasn't handler");
    return;
  }

  MessageSharedPointer message = new application_manager::Message(protocol_handler::MessagePriority::kDefault);//todo: ykazakov constant is a temp solution to finish merge MessagePriority::FromServiceType(message.servicetype) shall be used instead
  message->set_protocol_version(application_manager::ProtocolVersion::kHMI);
  message->set_smart_object(obj);
  handler()->OnMessageReceived(message);
  LOG4CXX_INFO(logger_, "Successfully sent to observer");
}

void DBusMessageAdapter::Request(const smart_objects::SmartObject& obj) {
  LOG4CXX_AUTO_TRACE(logger_);
  dbus::MessageId func_id = static_cast<dbus::MessageId>(
      obj[sos::S_PARAMS][sos::S_FUNCTION_ID].asInt());
  dbus::MessageName name = get_schema().getMessageName(func_id);
  uint id = obj[sos::S_PARAMS][sos::S_CORRELATION_ID].asInt();
  MethodCall(id, func_id, name, obj[sos::S_MSG_PARAMS]);
}

void DBusMessageAdapter::Notification(const smart_objects::SmartObject &obj) {
  LOG4CXX_AUTO_TRACE(logger_);
  dbus::MessageId func_id = static_cast<dbus::MessageId>(
      obj[sos::S_PARAMS][sos::S_FUNCTION_ID].asInt());
  dbus::MessageName name = get_schema().getMessageName(func_id);
  Signal(func_id, name, obj[sos::S_MSG_PARAMS]);
}

void DBusMessageAdapter::Response(const smart_objects::SmartObject& obj) {
  LOG4CXX_AUTO_TRACE(logger_);
  dbus::MessageId func_id = static_cast<dbus::MessageId>(
        obj[sos::S_PARAMS][sos::S_FUNCTION_ID].asInt());
  dbus::MessageName name = get_schema().getMessageName(func_id);
  uint id = obj[sos::S_PARAMS][sos::S_CORRELATION_ID].asInt();
  MethodReturn(id, func_id, name, obj[sos::S_MSG_PARAMS]);
}

void DBusMessageAdapter::ErrorResponse(const smart_objects::SmartObject &obj) {
  LOG4CXX_DEBUG(logger_, "Error");
  std::string error = obj[sos::S_PARAMS][sos::kCode].asString();
  std::string description = obj[sos::S_PARAMS][sos::kMessage].asString();
  uint id = obj[sos::S_PARAMS][sos::S_CORRELATION_ID].asInt();
  Error(id, error, description);
}

}  // namespace hmi_message_handler
