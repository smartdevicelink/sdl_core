/*
 * Copyright (c) 2017, Ford Motor Company
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

#include <string>

#include "hmi_message_handler/messagebroker_adapter.h"
#include "utils/logger.h"

namespace hmi_message_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "HMIMessageHandler")

typedef hmi_message_handler::CMessageBrokerController MessageBrokerController;

MessageBrokerAdapter::MessageBrokerAdapter(HMIMessageHandler* handler_param,
                                           const std::string& server_address,
                                           uint16_t port)
    : HMIMessageAdapterImpl(handler_param)
    , MessageBrokerController(server_address, port, "SDL", 8) {
  LOG4CXX_TRACE(logger_, "Created MessageBrokerAdapter");
}

MessageBrokerAdapter::~MessageBrokerAdapter() {}

void MessageBrokerAdapter::SendMessageToHMI(
    hmi_message_handler::MessageSharedPointer message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!message.valid()) {
    LOG4CXX_ERROR(logger_, "Can`t send not valid message");
    return;
  }

  Json::Reader reader;
  Json::Value json_value;
  if (!reader.parse(message->json_message(), json_value, false)) {
    LOG4CXX_ERROR(logger_, "Received invalid json string.");
    return;
  }

  sendJsonMessage(json_value);
}

void MessageBrokerAdapter::processResponse(std::string method,
                                           Json::Value& root) {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessRecievedFromMB(root);
}

void MessageBrokerAdapter::processRequest(Json::Value& root) {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessRecievedFromMB(root);
}

void MessageBrokerAdapter::processNotification(Json::Value& root) {
  LOG4CXX_AUTO_TRACE(logger_);
  ProcessRecievedFromMB(root);
}

void MessageBrokerAdapter::SubscribeTo() {
  LOG4CXX_INFO(logger_, "MessageBrokerAdapter::subscribeTo");
  MessageBrokerController::subscribeTo("Buttons.OnButtonEvent");
  MessageBrokerController::subscribeTo("Buttons.OnButtonPress");
  MessageBrokerController::subscribeTo("UI.OnCommand");
  MessageBrokerController::subscribeTo("VR.OnCommand");
  MessageBrokerController::subscribeTo("BasicCommunication.OnReady");
  MessageBrokerController::subscribeTo(
      "BasicCommunication.OnExitAllApplications");
  MessageBrokerController::subscribeTo("UI.OnDriverDistraction");
  MessageBrokerController::subscribeTo("UI.OnSystemContext");
  MessageBrokerController::subscribeTo("UI.OnAppActivated");
  MessageBrokerController::subscribeTo("UI.OnKeyboardInput");
  MessageBrokerController::subscribeTo("UI.OnTouchEvent");
  MessageBrokerController::subscribeTo("UI.OnResetTimeout");
  MessageBrokerController::subscribeTo("TTS.OnResetTimeout");
  MessageBrokerController::subscribeTo("BasicCommunication.OnAppDeactivated");
  MessageBrokerController::subscribeTo(
      "BasicCommunication.OnStartDeviceDiscovery");
  MessageBrokerController::subscribeTo("BasicCommunication.OnUpdateDeviceList");
  MessageBrokerController::subscribeTo("BasicCommunication.OnFindApplications");
  MessageBrokerController::subscribeTo("BasicCommunication.OnAppActivated");
  MessageBrokerController::subscribeTo("BasicCommunication.OnAwakeSDL");
  MessageBrokerController::subscribeTo("BasicCommunication.OnExitApplication");
  MessageBrokerController::subscribeTo(
      "BasicCommunication.OnExitAllApplications");
  MessageBrokerController::subscribeTo("BasicCommunication.OnDeviceChosen");
  MessageBrokerController::subscribeTo("UI.OnLanguageChange");
  MessageBrokerController::subscribeTo("VR.OnLanguageChange");
  MessageBrokerController::subscribeTo("TTS.OnLanguageChange");
  MessageBrokerController::subscribeTo("VehicleInfo.OnVehicleData");
  MessageBrokerController::subscribeTo("Navigation.OnTBTClientState");
  MessageBrokerController::subscribeTo("Navigation.OnWayPointChange");
  MessageBrokerController::subscribeTo("TTS.Started");
  MessageBrokerController::subscribeTo("TTS.Stopped");
  MessageBrokerController::subscribeTo("VR.Started");
  MessageBrokerController::subscribeTo("VR.Stopped");
  MessageBrokerController::subscribeTo("BasicCommunication.OnSystemRequest");
  MessageBrokerController::subscribeTo(
      "BasicCommunication.OnIgnitionCycleOver");
  MessageBrokerController::subscribeTo(
      "BasicCommunication.OnSystemInfoChanged");
  MessageBrokerController::subscribeTo("SDL.OnAppPermissionConsent");
  MessageBrokerController::subscribeTo("SDL.OnAllowSDLFunctionality");
  MessageBrokerController::subscribeTo("SDL.OnReceivedPolicyUpdate");
  MessageBrokerController::subscribeTo("SDL.OnSystemError");
  MessageBrokerController::subscribeTo("SDL.AddStatisticsInfo");
  MessageBrokerController::subscribeTo("SDL.OnDeviceStateChanged");
  MessageBrokerController::subscribeTo("SDL.OnPolicyUpdate");
  MessageBrokerController::subscribeTo("BasicCommunication.OnEventChanged");
  MessageBrokerController::subscribeTo("RC.OnInteriorVehicleData");
  MessageBrokerController::subscribeTo("RC.OnRemoteControlSettings");

  LOG4CXX_INFO(logger_, "Subscribed to notifications.");
}

void* MessageBrokerAdapter::SubscribeAndBeginReceiverThread(void* param) {
  PassToThread(threads::Thread::CurrentId());
  // For PASA we don't need to subscribe and register controller
  // this will prevent from errors on start up
  registerController();
  SubscribeTo();
  return MethodForReceiverThread(param);
}

void MessageBrokerAdapter::ProcessRecievedFromMB(Json::Value& root) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_INFO(logger_, "MB_Adapter: " << root);
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

  // Messages from HMI (sent through message broker) have no priority so far
  // assign default priority
  hmi_message_handler::MessageSharedPointer message =
      hmi_message_handler::MessageSharedPointer(
          new application_manager::Message(
              protocol_handler::MessagePriority::kDefault));
  // message->set_message_type()
  message->set_json_message(message_string);
  message->set_protocol_version(
      protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_HMI);

  if (!handler()) {
    LOG4CXX_WARN(logger_, "handler is NULL");
    return;
  }

  handler()->OnMessageReceived(message);
  LOG4CXX_INFO(logger_, "Successfully sent to observer");
}

}  // namespace hmi_message_handler
