/*

 Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/mobile/subscribe_button_request.h"
#include "application_manager/application_manager_impl.h"

namespace application_manager {

namespace commands {

namespace str = strings;

SubscribeButtonRequest::SubscribeButtonRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

SubscribeButtonRequest::~SubscribeButtonRequest() {
}

void SubscribeButtonRequest::Run() {
  LOG4CXX_INFO(logger_, "SubscribeButtonRequest::Run");

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "APPLICATION_NOT_REGISTERED");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const mobile_apis::ButtonName::eType btn_id =
      static_cast<mobile_apis::ButtonName::eType>(
          (*message_)[str::msg_params][str::button_name].asUInt());

  if (!IsSubscribtionAllowed(app, btn_id)) {
    LOG4CXX_ERROR_EXT(logger_, "Subscribe on button " << btn_id
                      << " isn't allowed");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  if (app->IsSubscribedToButton(btn_id)) {
    LOG4CXX_ERROR_EXT(logger_, "Already subscribed to button " << btn_id);
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  app->SubscribeToButton(static_cast<mobile_apis::ButtonName::eType>(btn_id));
  SendResponse(true, mobile_apis::Result::SUCCESS);

  app->UpdateHash();
}

bool SubscribeButtonRequest::IsSubscribtionAllowed(
    ApplicationSharedPtr app, mobile_apis::ButtonName::eType btn_id) {

  if (!app->is_media_application() &&
      ((mobile_apis::ButtonName::SEEKLEFT == btn_id) ||
       (mobile_apis::ButtonName::SEEKRIGHT == btn_id)||
       (mobile_apis::ButtonName::TUNEUP == btn_id)   ||
       (mobile_apis::ButtonName::TUNEDOWN == btn_id))) {
    return false;
  }

  return true;
}

}  // namespace commands

}  // namespace application_manager
