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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_REQUEST_FROM_HMI_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_REQUEST_FROM_HMI_H_

#include "application_manager/commands/command_impl.h"
#include "interfaces/HMI_API.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

namespace commands {

namespace ns_smart = ns_smart_device_link::ns_smart_objects;

class RequestFromHMI : public CommandImpl, public event_engine::EventObserver {
 public:
  RequestFromHMI(const MessageSharedPtr& message,
                 ApplicationManager& application_manager,
                 rpc_service::RPCService& rpc_service,
                 HMICapabilities& hmi_capabilities,
                 policy::PolicyHandlerInterface& policy_handler);
  virtual ~RequestFromHMI();
  virtual bool Init();
  virtual bool CleanUp();
  virtual void Run();
  virtual void on_event(const event_engine::Event& event);
  void on_event(const event_engine::MobileEvent& event) OVERRIDE;
  /**
   * @brief SendResponse allows to send response to hmi
   * @param correlation_id the correlation id for the rfesponse.
   * @param function_id the function id for which response will be sent
   * @param result_code the result code.
   */
  void SendResponse(
      const bool success,
      const uint32_t correlation_id,
      const hmi_apis::FunctionID::eType function_id,
      const hmi_apis::Common_Result::eType result_code,
      const smart_objects::SmartObject* response_params = NULL,
      commands::Command::CommandSource source = commands::Command::SOURCE_HMI);

  /**
   * @brief SendResponse allows to send error response to hmi
   * @param correlation_id the correlation id for the response.
   * @param function_id the function id for which response will be sent
   * @param result_code the result code.
   * @param error_message info message for error.
   */
  void SendErrorResponse(const uint32_t correlation_id,
                         const hmi_apis::FunctionID::eType function_id,
                         const hmi_apis::Common_Result::eType result_code,
                         const std::string error_message);

  void SendProviderRequest(
      const mobile_apis::FunctionID::eType& mobile_function_id,
      const hmi_apis::FunctionID::eType& hmi_function_id,
      const smart_objects::SmartObject* msg,
      bool use_events = false);

  void SendMobileRequest(const mobile_apis::FunctionID::eType& function_id,
                         const ApplicationSharedPtr app,
                         const smart_objects::SmartObject* msg_params,
                         bool use_events);

  void SendHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                      const smart_objects::SmartObject* msg_params,
                      bool use_events);

  bool ProcessHMIInterfacesAvailability(
      const uint32_t hmi_correlation_id,
      const hmi_apis::FunctionID::eType& function_id);

 protected:
  bool IsMobileResultSuccess(mobile_apis::Result::eType result_code) const;

  bool IsHMIResultSuccess(hmi_apis::Common_Result::eType result_code,
                          HmiInterfaces::InterfaceID interface) const;

 private:
  /**
   * @brief Fills common parameters for SO
   * @param message Contains SO for filling
   * @param correlation_id the correlation id for the response.
   * @param function_id the function id for which response will be sent
   */
  void FillCommonParametersOfSO(
      ns_smart_device_link::ns_smart_objects::SmartObject& message,
      const uint32_t correlation_id,
      const hmi_apis::FunctionID::eType function_id);

 private:
  DISALLOW_COPY_AND_ASSIGN(RequestFromHMI);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_REQUEST_FROM_HMI_H_
