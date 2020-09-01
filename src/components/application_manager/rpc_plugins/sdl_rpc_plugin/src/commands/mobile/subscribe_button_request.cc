/*

 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/commands/mobile/subscribe_button_request.h"
#include "utils/semantic_version.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

namespace str = strings;

SubscribeButtonRequest::SubscribeButtonRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

SubscribeButtonRequest::~SubscribeButtonRequest() {}

void SubscribeButtonRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "APPLICATION_NOT_REGISTERED");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  mobile_apis::ButtonName::eType btn_id =
      static_cast<mobile_apis::ButtonName::eType>(
          (*message_)[str::msg_params][str::button_name].asInt());

  if (!IsSubscriptionAllowed(app, btn_id)) {
    LOG4CXX_ERROR(logger_,
                  "Subscribe on button " << btn_id << " isn't allowed");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  if (app->msg_version() < utils::rpc_version_5 &&
      btn_id == mobile_apis::ButtonName::OK && app->is_media_application()) {
    bool ok_supported = CheckHMICapabilities(mobile_apis::ButtonName::OK);
    bool play_pause_supported =
        CheckHMICapabilities(mobile_apis::ButtonName::PLAY_PAUSE);
    if (play_pause_supported) {
      LOG4CXX_DEBUG(logger_, "Converting Legacy OK button to PLAY_PAUSE");
      btn_id = mobile_apis::ButtonName::PLAY_PAUSE;
      (*message_)[str::msg_params][str::button_name] = btn_id;
    } else if (!ok_supported) {
      LOG4CXX_ERROR(logger_, "OK button isn't allowed by HMI capabilities");
      SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
    }
  } else if (!CheckHMICapabilities(btn_id)) {
    LOG4CXX_ERROR(logger_,
                  "Subscribe on button "
                      << btn_id << " isn't allowed by HMI capabilities");
    SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
    return;
  }

  if (app->IsSubscribedToButton(btn_id)) {
    LOG4CXX_ERROR(logger_, "Already subscribed to button " << btn_id);
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  app->SubscribeToButton(static_cast<mobile_apis::ButtonName::eType>(btn_id));
  SendSubscribeButtonNotification();

  const bool is_succeeded = true;
  SendResponse(is_succeeded, mobile_apis::Result::SUCCESS);
}

bool SubscribeButtonRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

bool SubscribeButtonRequest::IsSubscriptionAllowed(
    ApplicationSharedPtr app, mobile_apis::ButtonName::eType btn_id) {
  if (!app->is_media_application() &&
      ((mobile_apis::ButtonName::PLAY_PAUSE == btn_id) ||
       (mobile_apis::ButtonName::SEEKLEFT == btn_id) ||
       (mobile_apis::ButtonName::SEEKRIGHT == btn_id) ||
       (mobile_apis::ButtonName::TUNEUP == btn_id) ||
       (mobile_apis::ButtonName::TUNEDOWN == btn_id))) {
    return false;
  }

  if (!app->is_navi() &&
      ((mobile_apis::ButtonName::NAV_CENTER_LOCATION == btn_id) ||
       (mobile_apis::ButtonName::NAV_ZOOM_IN == btn_id) ||
       (mobile_apis::ButtonName::NAV_ZOOM_OUT == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_UP == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_UP_RIGHT == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_RIGHT == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_DOWN_RIGHT == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_DOWN == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_DOWN_LEFT == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_LEFT == btn_id) ||
       (mobile_apis::ButtonName::NAV_PAN_UP_LEFT == btn_id) ||
       (mobile_apis::ButtonName::NAV_TILT_TOGGLE == btn_id) ||
       (mobile_apis::ButtonName::NAV_ROTATE_CLOCKWISE == btn_id) ||
       (mobile_apis::ButtonName::NAV_ROTATE_COUNTERCLOCKWISE == btn_id) ||
       (mobile_apis::ButtonName::NAV_HEADING_TOGGLE == btn_id))) {
    return false;
  }

  return true;
}

void SubscribeButtonRequest::SendSubscribeButtonNotification() {
  using namespace smart_objects;
  using namespace hmi_apis;

  // send OnButtonSubscription notification
  SmartObject msg_params = SmartObject(SmartType_Map);
  msg_params[strings::app_id] = connection_key();
  msg_params[strings::name] = static_cast<Common_ButtonName::eType>(
      (*message_)[strings::msg_params][strings::button_name].asUInt());
  msg_params[strings::is_suscribed] = true;
  CreateHMINotification(FunctionID::Buttons_OnButtonSubscription, msg_params);
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
