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

#include "rpc/hmi_message_handler_tester.h"

namespace test {
  HMIMessageHandlerTester::HMIMessageHandlerTester()
    : cond_var_(),
      aLock_(lock_) {
    hmi_impl_ = hmi_message_handler::HMIMessageHandlerImpl::instance();
    DCHECK(hmi_impl_ != NULL)
  }

  HMIMessageHandlerTester::~HMIMessageHandlerTester() {
  }

  HMIMessageHandlerTester::HMIMessageHandlerTester(
      const HMIMessageHandlerTester&)
    : cond_var_(),
      aLock_(lock_) {
  }

  HMIMessageHandlerTester* HMIMessageHandlerTester::operator=(
      const HMIMessageHandlerTester&) {
    return this;
  }

  HMIMessageHandlerTester* HMIMessageHandlerTester::instance() {
    static HMIMessageHandlerTester instance;
    return &instance;
  }

  void HMIMessageHandlerTester::SignalNewRequestFromMobile() {
    while (!mas_request_to_hmi_.empty()) {
      mas_untestable_request_to_hmi.push_back(mas_request_to_hmi_.front());
      mas_request_to_hmi_.pop_front();
    }
  }

  void HMIMessageHandlerTester::SendMessageToHMI(
      hmi_message_handler::MessageSharedPointer message) {
    printf("\n\n\n\n  HMIMessageHandlerTester::SendMessageToHMI \n\n\n\n");

    mas_request_to_hmi_.push_back(message);

    printf("correlation_id : %d\n", (*message).correlation_id());
    printf("function_id : %d\n", (*message).function_id());
    printf("connection_key : %d\n", (*message).connection_key());
    printf("type : %d\n", (*message).type());
    printf("protocol_version : %d\n", (*message).protocol_version());
    printf("json_message : %s\n\n", (*message).json_message().data());

    hmi_impl_->SendMessageToHMI(message);
  }

  void HMIMessageHandlerTester::OnMessageReceived(
      utils::SharedPtr<application_manager::Message> message) {
    hmi_impl_->OnMessageReceived(message);
    printf("\n\n\n\n  HMIMessageHandlerTester::OnMessageReceived \n\n\n\n");
  }

  void HMIMessageHandlerTester::OnErrorSending(
      utils::SharedPtr<application_manager::Message> message) {
    hmi_impl_->OnErrorSending(message);
    printf("\n\n\n\n  HMIMessageHandlerTester::OnErrorSending \n\n\n\n");
  }

  void HMIMessageHandlerTester::AddHMIMessageAdapter(
      hmi_message_handler::HMIMessageAdapter* adapter) {
    hmi_impl_->AddHMIMessageAdapter(adapter);
    printf("\n\n\n\n  HMIMessageHandlerTester::AddHMIMessageAdapter \n\n\n\n");
  }

  void HMIMessageHandlerTester::RemoveHMIMessageAdapter(
      hmi_message_handler::HMIMessageAdapter* adapter) {
    hmi_impl_->RemoveHMIMessageAdapter(adapter);
    printf("\n\n\n\n  "
        "HMIMessageHandlerTester::RemoveHMIMessageAdapter \n\n\n\n");
  }

  bool HMIMessageHandlerTester::operator()(
      utils::SharedPtr<smart::SmartObject> message) {
    bool returnValues = true;

    if (mas_request_to_hmi_.empty()) {
      return false;
    }

    hmi_message_handler::MessageSharedPointer* mes =
        &mas_request_to_hmi_.front();

    smart::SmartObject object;
    NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2
        ::fromString(
            (*mes)->json_message().data(),
            object,
            (*mes)->function_id(),
            mobile_apis::messageType::request);
    printf("\nRequest to HMI");
    application_manager::MessageHelper::PrintSmartObject(object);

    printf("\nRequest from XML");
    application_manager::MessageHelper::PrintSmartObject(*message);
    /*
    if ((*message)["msg_params"]["params"].keyExists("cmdIcon")) {
      object["msg_params"]["params"]["cmdIcon"]["value"] =
      (*message)["msg_params"]["params"]["cmdIcon"]["value"].asString();
      // printf("\n\n blabla %s\n\n\n",object["msg_params"]["params"]
         ["cmdIcon"]["value"].asString().c_str());
    }
    */
    returnValues = returnValues &&
        (object["params"]["function_id"] ==
            (*message)["params"]["function_id"]);
    returnValues = returnValues &&
        (object["msg_params"]["method"] ==
            (*message)["msg_params"]["method"]);
    returnValues = returnValues &&
        (object["msg_params"]["params"] ==
            (*message)["msg_params"]["params"]);

    mas_request_to_hmi_.pop_front();
    mes = NULL;

    return returnValues;
  }

  bool HMIMessageHandlerTester::CheckUntestableRequest() {
    if (mas_untestable_request_to_hmi.empty()) {
     return true;
    }

    std::list<hmi_message_handler::MessageSharedPointer>::iterator iter =
        mas_untestable_request_to_hmi.begin();
    for (; iter != mas_untestable_request_to_hmi.end(); iter++) {
      smart::SmartObject object;
      NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2
          ::fromString(
              (iter->get())->json_message().data(),
              object,
              (iter->get())->function_id(),
              mobile_apis::messageType::request);
      application_manager::MessageHelper::PrintSmartObject(object);
    }

    return false;
  }
}  // namespace test


