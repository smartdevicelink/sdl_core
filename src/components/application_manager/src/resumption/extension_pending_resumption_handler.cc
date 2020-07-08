/*
 Copyright (c) 2020, Ford Motor Company
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

#include "application_manager/resumption/extension_pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "smart_objects/smart_object.h"

namespace resumption {

namespace app_mngr = application_manager;

CREATE_LOGGERPTR_GLOBAL(logger_, "ExtensionPenndingResumptionHandler")

ExtensionPendingResumptionHandler::ExtensionPendingResumptionHandler(
    app_mngr::ApplicationManager& application_manager)
    : application_manager::event_engine::EventObserver(
          application_manager.event_dispatcher())
    , application_manager_(application_manager) {}

ResumptionRequest ExtensionPendingResumptionHandler::MakeResumptionRequest(
    const uint32_t corr_id,
    const hmi_apis::FunctionID::eType function_id,
    const smart_objects::SmartObject& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  resumption::ResumptionRequest resumption_request;
  resumption_request.request_ids.correlation_id = corr_id;
  resumption_request.request_ids.function_id = function_id;
  resumption_request.message = message;
  return resumption_request;
}

}  //  namespace resumption