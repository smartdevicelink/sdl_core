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

#include "application_manager/commands/hmi/activate_app_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"

namespace application_manager {

  namespace commands {

    ActivateAppRequest::ActivateAppRequest(
        const MessageSharedPtr& message): RequestToHMI(message) {
    }


    ActivateAppRequest::~ActivateAppRequest() {
      LOG4CXX_TRACE(logger_, "~ActivateAppRequest");
    }

    void ActivateAppRequest::Run() {
      LOG4CXX_TRACE(logger_, "enter " << correlation_id());
      uint32_t app_id = RequestToHMI::application_id();
      ApplicationManagerImpl::instance()->set_application_id(correlation_id(), app_id);
#ifdef ENABLE_LOG
      if ((*message_)[strings::msg_params].keyExists(
            strings::activate_app_hmi_level)) {
        mobile_apis::HMILevel::eType requested_hmi_level =
            static_cast<mobile_apis::HMILevel::eType>(
                   (*message_)[strings::msg_params][strings::activate_app_hmi_level].asInt());
        LOG4CXX_TRACE(logger_, "requested_hmi_level = "
                        << requested_hmi_level);
      }
#endif
      SendRequest();
      LOG4CXX_TRACE(logger_, "exit");
    }

  }  // namespace commands

}  // namespace application_manager

