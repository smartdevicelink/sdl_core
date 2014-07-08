/**
 * \file protocol_handler.h
 * \brief ProtocolHandlerImpl class header file.
 *
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

#include "rpc/protocol_handler_mockup.h"

namespace test {
  ProtocolHandlerMockup::ProtocolHandlerMockup(
      transport_manager::TransportManager* transport_manager_param)
    : ProtocolHandlerImpl(transport_manager_param),
      cond_var_(),
      a_lock_(lock_),
      response_from_hmi_() {
  }

  ProtocolHandlerMockup::~ProtocolHandlerMockup() {
  }

  ProtocolHandlerMockup* ProtocolHandlerMockup::instance() {
    transport_manager::TransportManager* transport_manager_;
    transport_manager_ = transport_manager::TransportManagerDefault::instance();

    if (!transport_manager_) {
      return NULL;
    }

    static ProtocolHandlerMockup instance(transport_manager_);
    return &instance;
  }

  ProtocolHandlerMockup* ProtocolHandlerMockup::operator=(
      const ProtocolHandlerMockup &) {
    return this;
  }

  ProtocolHandlerMockup::ProtocolHandlerMockup(const ProtocolHandlerMockup &)
    : ProtocolHandlerImpl(NULL),
      cond_var_(),
      a_lock_(lock_) {
  }

  void ProtocolHandlerMockup::SendMessageToMobileApp(
      const RawMessagePtr message, bool final_message) {
    printf("\n\n\n SendMessageToMobileApp \n\n\n");

    utils::SharedPtr<application_manager::Message> shared_prt_mes;
    shared_prt_mes =
        application_manager::MobileMessageHandler::
        HandleIncomingMessageProtocolV2(message);

    printf("correlation_id : %d\n", (*shared_prt_mes).correlation_id());
    printf("function_id : %d\n", (*shared_prt_mes).function_id());
    printf("connection_key : %d\n", (*shared_prt_mes).connection_key());
    printf("type : %d\n", (*shared_prt_mes).type());
    printf("protocol_version : %d\n", (*shared_prt_mes).protocol_version());
    printf("json_message : %s\n\n", (*shared_prt_mes).json_message().data());

    if ((*shared_prt_mes).json_message().find("resultCode") !=
        std::string::npos) {
      response_from_hmi_ = shared_prt_mes;
      cond_var_.NotifyOne();
    }
  }

  bool ProtocolHandlerMockup::operator()(
      utils::SharedPtr<smart::SmartObject> in_request,
      utils::SharedPtr<smart::SmartObject> in_response) {
    bool return_values = true;
    sleep(3);

    HMIMessageHandlerTester* hmi_handler_test =
        HMIMessageHandlerTester::instance();

    hmi_handler_test->SignalNewRequestFromMobile();

    request_  = in_request;
    response_ = in_response;
    printf("SO from mobile");
    application_manager::MessageHelper::PrintSmartObject(*in_request);

    application_manager::ApplicationManagerImpl* app_manager_ =
      application_manager::ApplicationManagerImpl::instance();

    // app_manager_->set_protocol_handler(this);
    // app_manager_->ManageMobileCommand(request_);//dont work in all test

    application_manager::Message* mes = new application_manager::Message(
        protocol_handler::MessagePriority::kDefault);

    utils::SharedPtr<application_manager::Message> message_to_app(mes);
    std::string str;

    (*message_to_app).set_correlation_id(
        (*request_)[jsn::S_PARAMS][am::strings::correlation_id].asInt());
    (*message_to_app).set_function_id(
        (*request_)[jsn::S_PARAMS][am::strings::function_id].asInt());
    (*message_to_app).set_connection_key(
        (*request_)[jsn::S_PARAMS][am::strings::connection_key].asInt());
    (*message_to_app).set_message_type(application_manager::kRequest);
    (*message_to_app).set_protocol_version(
        static_cast<application_manager::ProtocolVersion>
            ((*request_)[jsn::S_PARAMS][am::strings::protocol_version].asInt()));
    NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
        toString((*request_), str);

    (*message_to_app).set_json_message(str);

    utils::SharedPtr<protocol_handler::RawMessage> raw_mes(
        am::MobileMessageHandler::HandleOutgoingMessageProtocolV2(
            message_to_app));

     // app_manager_->OnMessageReceived(raw_mes);
     // app_manager_->ManageMobileCommand(request_);

    if ((*request_)[jsn::S_PARAMS].keyExists("binary_data")) {
      app_manager_->ManageMobileCommand(request_);
    } else {
      app_manager_->OnMessageReceived(raw_mes);
    }

    cond_var_.WaitFor(a_lock_, 10000);

    std::string str_result_code;
    Json::Value value;
    Json::Reader reader;

    reader.parse((*response_from_hmi_).json_message(), value);
    Json::Value result_code = value.get("resultCode", "");
    str_result_code = result_code.asString();

    printf("\n\n\nXMLResponce correlation_id :%d",
           (*response_)[jsn::S_PARAMS][am::strings::correlation_id].asInt());
    printf(" XMLResponce result_code :%s\n",
           ((*response_)[jsn::S_MSG_PARAMS][am::strings::result_code].
               asString()).c_str());

    printf("HMIResponce correlation_id : %d HMIResponce result_code : %s\n\n\n",
           (*response_from_hmi_).correlation_id(), str_result_code.c_str());

    if ((*response_from_hmi_).correlation_id() !=
        (*response_)[jsn::S_PARAMS][am::strings::correlation_id].asInt()) {
      return_values = false;
    }
    if (str_result_code !=
        (*response_)[jsn::S_MSG_PARAMS][am::strings::result_code].asString()) {
      return_values = false;
    }
    sleep(1);

    return return_values;
  }
}  // namespace test


