/*
 Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_

#include "application_manager/commands/command_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "utils/lock.h"
#include "smart_objects/smart_object.h"

namespace application_manager {
namespace commands {

struct ResponseInfo {
  DEPRECATED ResponseInfo(hmi_apis::Common_Result::eType result,
                          HmiInterfaces::InterfaceID interface);
  ResponseInfo();
  ResponseInfo(const hmi_apis::Common_Result::eType result,
               const HmiInterfaces::InterfaceID hmi_interface,
               ApplicationManager& application_manager);
  hmi_apis::Common_Result::eType result_code;
  HmiInterfaces::InterfaceID interface;
  HmiInterfaces::InterfaceState interface_state;
  bool is_ok;
  bool is_unsupported_resource;
  bool is_not_used;
};

namespace NsSmart = NsSmartDeviceLink::NsSmartObjects;

/**
 * @brief MergeInfos merge 2 infos in one string
 * @param first - info string that should be first in result info
 * @param second - info string that should be second in result info
 * @return if first is empty return second
 *         if second is empty return first
 *         if both are empty return empty string
 *         if both are not empty return empty first +", " + second
 */
std::string MergeInfos(const std::string& first, const std::string& second);

/**
 * @brief MergeInfos merge 2 infos into one string with info
 * @param first_info -contains result_code from HMI response and
 * interface that returns response
 * @param first_str - info string that should be first in result info
 * @param second_info -contains result_code from HMI response and
 * interface that returns response
 * @param second_str - info string that should be second in result info
 * @return if first_info is not available and second_str not empty return second
 *         if second_info is not available and first_str not empty return first
 *         other cases return result MergeInfos for 2 params
 */
std::string MergeInfos(const ResponseInfo& first_info,
                       const std::string& first_str,
                       const ResponseInfo& second_info,
                       const std::string& second_str);

/**
 * @brief MergeInfos merge 3 infos in one string
 * @param first - info string that should be first in result info
 * @param second - info string that should be second in result info
 * @param third - info string that should be second in result info
 * @return resulting string contain merge all incoming parameters
 */
std::string MergeInfos(const std::string& first,
                       const std::string& second,
                       const std::string& third);

class CommandRequestImpl : public CommandImpl,
                           public event_engine::EventObserver {
 public:
  enum RequestState { kAwaitingHMIResponse = 0, kTimedOut, kCompleted };

  /**
   * @brief The HashUpdateMode enum defines whether request has to update
   * hash after its execution is finished
   */
  enum HashUpdateMode { kSkipHashUpdate, kDoHashUpdate };

  CommandRequestImpl(const MessageSharedPtr& message,
                     ApplicationManager& application_manager,
                     rpc_service::RPCService& rpc_service,
                     HMICapabilities& hmi_capabilities,
                     policy::PolicyHandlerInterface& policy_handler);

  ~CommandRequestImpl();

  /**
   * @brief Checks command permissions according to policy table
   */
  bool CheckPermissions() OVERRIDE;

  /**
   * @brief Init sets hash update mode for request
   */
  bool Init() OVERRIDE;

  /**
   * @brief Cleanup all resources used by command
   **/
  bool CleanUp() OVERRIDE;

  /**
   * @brief Execute corresponding command by calling the action on reciever
   **/
  void Run() OVERRIDE;

  /*
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   *
   */
  virtual void onTimeOut();

  /**
   * @brief Default EvenObserver's pure virtual method implementation
   *
   * @param event The received event
   */
  virtual void on_event(const event_engine::Event& event);

  /*
   * @brief Creates Mobile response
   *
   * @param success true if successful; false, if failed
   * @param result_code Result code (SUCCESS, INVALID_DATA, e.t.c)
   * @param info Provides additional human readable info regarding the result
   * @param response_params Additional params in response
   */
  void SendResponse(const bool success,
                    const mobile_apis::Result::eType& result_code,
                    const char* info = NULL,
                    const smart_objects::SmartObject* response_params = NULL);

  /**
   * @brief Check syntax of string from mobile
   * @param str - string that need to be checked
   * @param allow_empty_string if true methods allow empty sting
   * @return true if success otherwise return false
   */
  bool CheckSyntax(const std::string& str, bool allow_empty_line = false);

  /*
   * @brief Sends HMI request
   *
   * @param function_id HMI request ID
   * @param msg_params HMI request msg params
   * @param use_events true if we need subscribe on event(HMI request)
   * @return hmi correlation id
   */
  uint32_t SendHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                          const smart_objects::SmartObject* msg_params = NULL,
                          bool use_events = false);

  /*
   * @brief Creates HMI request
   *
   * @param function_id HMI request ID
   * @param msg_params HMI request msg params
   */
  void CreateHMINotification(const hmi_apis::FunctionID::eType& function_id,
                             const NsSmart::SmartObject& msg_params) const;

  /**
   * @brief Converts HMI result code to Mobile result code
   *
   * @param hmi_code HMI result code
   * @return Mobile result code
   */
  mobile_apis::Result::eType GetMobileResultCode(
      const hmi_apis::Common_Result::eType& hmi_code) const;

 protected:
  /**
   * @brief Checks message permissions and parameters according to policy table
   * permissions
   */
  bool CheckAllowedParameters();

  /**
   * @brief Checks HMI capabilities for specified button support
   * @param button Button to check
   * @return true if button is present in HMI capabilities
   * otherwise returns false
   */
  bool CheckHMICapabilities(const mobile_apis::ButtonName::eType button) const;

  /**
   * @brief Remove from current message parameters disallowed by policy table
   */
  void RemoveDisallowedParameters();

  /**
   * @brief Adds disallowed parameters back to response with appropriate
   * reasons
   * @param response Response message, which should be extended with blocked
   * parameters reasons
   */
  void AddDisallowedParameters(smart_objects::SmartObject& response);

  /**
   * @brief Checks if any request param was marked as disallowed by policy
   * @return true if any param was marked as disallowed
   */
  bool HasDisallowedParams() const;

  /**
   * @brief Checks result code from HMI for single RPC
   * and returns parameter for sending to mobile app.
   * @param result_code contains result code from HMI response
   * @param interface contains interface for which HMI sent response
   * @return true if result code complies successful result cods
   * otherwise returns false.
   */
  bool PrepareResultForMobileResponse(
      hmi_apis::Common_Result::eType result_code,
      HmiInterfaces::InterfaceID interface) const;

  /**
   * @brief Checks result code from HMI for splitted RPC
   * and returns parameter for sending to mobile app.
   * @param first contains result_code from HMI response and
   * interface that returns response
   * @param second contains result_code from HMI response and
   * interface that returns response
   * @return true if result code complies successful result code
   * otherwise returns false
   */
  bool PrepareResultForMobileResponse(ResponseInfo& out_first,
                                      ResponseInfo& out_second) const;

  /**
   * @brief If message from HMI contains returns this info
   * or process result code from HMI and checks state of interface
   * and create info.
   * @param interface contains interface for which HMI sent response
   * @param result_code contains result code from HMI
   * @param response_from_hmi contains response from HMI
   * @param out_info contain info for sending to application
   */
  void GetInfo(const smart_objects::SmartObject& response_from_hmi,
               std::string& out_info);

  /**
   * @brief Prepare result code for sending to mobile application
   * @param first contains result_code from HMI response and
   * interface that returns response
   * @param second contains result_code from HMI response and
   * interface that returns response.
   * @return resulting code for sending to mobile application.
   */
  mobile_apis::Result::eType PrepareResultCodeForResponse(
      const ResponseInfo& first, const ResponseInfo& second);

  /**
   * @brief Resolves if the return code must be
   * UNSUPPORTED_RESOURCE
   * @param first contains result_code from HMI response and
   * interface that returns response
   * @param second contains result_code from HMI response and
   * interface that returns response.
   * @return True, if the communication return code must be
   * UNSUPPORTED_RESOURCE, otherwise false.
   */
  bool IsResultCodeUnsupported(const ResponseInfo& first,
                               const ResponseInfo& second) const;

 protected:
  /**
   * @brief Returns policy parameters permissions
   * @return Parameters permissions struct reference
   */
  const CommandParametersPermissions& parameters_permissions() const;

  /**
   * @brief Adds interface to be awaited for by sdl request command
     @param interface_id interface which SDL expects to response in given time
  */
  void StartAwaitForInterface(const HmiInterfaces::InterfaceID interface_id);

  /**
   * @brief Gets interface await state.
   * @param interface_id interface which SDL awaits for response in given time
   * @return true if SDL awaits for response from given interface in
   * interface_id
  */
  bool IsInterfaceAwaited(const HmiInterfaces::InterfaceID& interface_id) const;

  /**
   * @brief Sets given HMI interface await status to false
   * @param interface_id interface which SDL no longer awaits for response in
   * given time
   */
  void EndAwaitForInterface(const HmiInterfaces::InterfaceID& interface_id);

  /**
  * @brief This set stores all the interfaces which are awaited by SDL to
   * return a response on some request
   */
  std::set<HmiInterfaces::InterfaceID> awaiting_response_interfaces_;

  mutable sync_primitives::Lock awaiting_response_interfaces_lock_;

  RequestState current_state_;
  sync_primitives::Lock state_lock_;
  CommandParametersPermissions parameters_permissions_;
  CommandParametersPermissions removed_parameters_permissions_;

  /**
   * @brief hash_update_mode_ Defines whether request must update hash value of
   * application or not
   */
  HashUpdateMode hash_update_mode_;

 private:
  DISALLOW_COPY_AND_ASSIGN(CommandRequestImpl);

  /**
   * @brief Adds param to disallowed parameters enumeration
   * @param info string with disallowed params enumeration
   * @param param disallowed param
   */
  void AddDissalowedParameterToInfoString(std::string& info,
                                          const std::string& param) const;

  /**
   * @brief Adds disallowed parameters to response info
   * @param response Response message, which info should be extended
   */
  void AddDisallowedParametersToInfo(
      smart_objects::SmartObject& response) const;

  bool ProcessHMIInterfacesAvailability(
      const uint32_t hmi_correlation_id,
      const hmi_apis::FunctionID::eType& function_id);

  /**
    * @brief UpdateHash updates hash field for application and sends
    * OnHashChanged notification to mobile side in case of approriate hash mode
    * is set
    */
  void UpdateHash();

  /**
   * @brief is_success_result_ Defines whether request succeded, at the moment
   * it is value of 'success' field of appropriate response sent to mobile
   */
  bool is_success_result_;

  /**
   * @brief Add information for the component of response in case of timeout
   * @param response Response message, which info should be extended
   */
  void AddTimeOutComponentInfoToMessage(
      smart_objects::SmartObject& response) const;
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_REQUEST_IMPL_H_
