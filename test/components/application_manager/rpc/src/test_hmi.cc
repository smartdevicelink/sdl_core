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

#include <unistd.h>
#include "application_manager/application_impl.h"

#include "rpc/test_hmi.h"

namespace test {
  namespace app_manager_test {
    void TestHMI() {
      printf("\n\n\n init param to HMI AddCommand \n\n\n");

      connection_handler::ConnectionHandlerImpl* connection_handler_ =
          connection_handler::ConnectionHandlerImpl::instance();

      application_manager::ApplicationManagerImpl* app_manager_ =
          application_manager::ApplicationManagerImpl::instance();

      utils::SharedPtr<am::Application> appl(app_manager_->application(65537));

      if (!appl.valid()) {
        RegApp();
        sleep(5);
        appl = app_manager_->application(65537);

        if (!appl.valid()) {
          printf("\n\n\n No register app  \n\n\n");
          return;
        }
      }
      ////////////////////////////////////////
      sleep(5);

      printf("\n\n\n AddCommand Request  \n\n\n");

      HMIMessageHandlerTester* hmi_handler_test;

      hmi_handler_test = HMIMessageHandlerTester::instance();

      utils::SharedPtr<smart::SmartObject> HMIRequest(new smart::SmartObject);

      (*HMIRequest)[jsn::S_PARAMS][am::strings::function_id] =
          mobile_apis::FunctionID::AddCommandID;
      // (*HMIRequest)[S_PARAMS][strings::correlation_id] = 3;
      (*HMIRequest)[jsn::S_PARAMS][am::strings::message_type] =
          mobile_apis::messageType::request;
      (*HMIRequest)[jsn::S_PARAMS][am::strings::protocol_type] = 0;
      (*HMIRequest)[jsn::S_PARAMS][am::strings::protocol_version] = 2;
      (*HMIRequest)[jsn::S_PARAMS][am::strings::connection_key] = 65537;

      // (*HMIRequest)[S_MSG_PARAMS][strings::app_id] = "12345";
      (*HMIRequest)[jsn::S_MSG_PARAMS][am::strings::app_name] =
          "SyncProxyTester";

      app_manager_->set_hmi_message_handler(hmi_handler_test);

      // appl->set_hmi_level(mobile_apis::HMILevel::HMI_BACKGROUND);

      app_manager_->ManageMobileCommand(HMIRequest);

      sleep(5);

      hmi_message_handler::HMIMessageHandlerImpl* hmi_handler_;

      hmi_handler_ = hmi_message_handler::HMIMessageHandlerImpl::instance();
      DCHECK(hmi_handler_ != NULL)

      app_manager_->set_hmi_message_handler(hmi_handler_);
    }
  }  // namespace app_manager_test
}  // namespace test
