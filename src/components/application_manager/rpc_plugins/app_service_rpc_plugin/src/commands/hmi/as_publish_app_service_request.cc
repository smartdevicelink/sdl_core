/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "app_service_rpc_plugin/commands/hmi/as_publish_app_service_request.h"
#include "application_manager/message_helper.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

ASPublishAppServiceRequest::ASPublishAppServiceRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromHMI(message,
                     application_manager,
                     rpc_service,
                     hmi_capabilities,
                     policy_handler) {}

ASPublishAppServiceRequest::~ASPublishAppServiceRequest() {}

void ASPublishAppServiceRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject service_record =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  service_record[strings::service_manifest] =
      (*message_)[strings::msg_params][strings::app_service_manifest];
  service_record[strings::service_id] = "This is a service ID";
  service_record[strings::service_published] = true;
  service_record[strings::service_active] = true;
  response_params[strings::app_service_record] = service_record;
  // TODO: Add AppServiceRecord to response
  SendResponse(true,
               (*message_)[strings::params][strings::correlation_id].asUInt(),
               hmi_apis::FunctionID::AppService_PublishAppService,
               hmi_apis::Common_Result::SUCCESS,
               &response_params);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin
