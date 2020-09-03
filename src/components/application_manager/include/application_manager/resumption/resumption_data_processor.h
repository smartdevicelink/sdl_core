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
#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_PROCESSOR_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_PROCESSOR_H_

#include "application_manager/application.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "smart_objects/smart_object.h"

namespace resumption {

/**
 * @brief Determines whether response is successful
 * judging from type of response received from HMI
 * @param response from HMI
 */
bool IsResponseSuccessful(const smart_objects::SmartObject& response);

/**
 * @brief The ResumptionRequestID struct contains fields, needed during
 * processing events, related to responses from HMI to each resumption request
 */
struct ResumptionRequestID {
  hmi_apis::FunctionID::eType function_id;
  int32_t correlation_id;

  /**
   * @brief This operator is needed for correct iteration through map, which
   * contains keys with type ResumptionRequestID
   */
  bool operator<(const ResumptionRequestID& other) const;
};

/**
 * @brief The ResumptionRequest struct contains information, needed for
 * processing event, and request message
 */
struct ResumptionRequest {
  ResumptionRequestID request_id;
  smart_objects::SmartObject message;
};

/**
 * @brief Contains logic for the resumption and revert resumption data of
 *  applications.
 */
class ResumptionDataProcessor {
 public:
  /**
   * @brief Running resumption data process from saved_app to application.
   * @param application application which will be resumed
   * @param saved_app application specific section from backup file
   * @param callback function signature to be called when
   * data resumption will be finished
   */
  virtual void Restore(application_manager::ApplicationSharedPtr application,
                       smart_objects::SmartObject& saved_app,
                       ResumeCtrl::ResumptionCallBack callback) = 0;

  /**
   * @brief Handle restored data when timeout appeared
   * @param correlation_id Correlation ID from event
   * @param function_id Function ID from event
   */
  virtual void HandleOnTimeOut(
      const uint32_t correlation_id,
      const hmi_apis::FunctionID::eType function_id) = 0;

  /**
   * @brief subscribe to events for the application and save request
   * @param app_id application id
   * @param request for saving
   */
  virtual void SubscribeToResponse(const int32_t app_id,
                                   const ResumptionRequest& request) = 0;
};

}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_PROCESSOR_H_
