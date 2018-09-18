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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "utils/macro.h"
#include "utils/custom_string.h"

namespace policy {
struct DeviceInfo;
}

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

class Application;

namespace commands {

namespace custom_str = utils::custom_string;

/**
 * @brief Register app interface request  command class
 **/
class RegisterAppInterfaceRequest
    : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * \brief RegisterAppInterfaceRequest class constructor
   **/
  RegisterAppInterfaceRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief RegisterAppInterfaceRequest class destructor
   **/
  virtual ~RegisterAppInterfaceRequest();

  /**
   * @brief Init required by command resources
   **/
  virtual bool Init();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief onTimeOut from requrst Controller
   **/
  void onTimeOut() OVERRIDE;

  /**
   * @brief Prepares and sends RegisterAppInterface response to mobile
   * considering application type
   **/
  void SendRegisterAppInterfaceResponseToMobile();

 private:
  /**
   * @brief FillApplicationParams set app application attributes from the RAI
   * request
   * @param application applicaiton to fill params
   */
  void FillApplicationParams(
      application_manager::ApplicationSharedPtr application);

  /**
   * @brief The AppicationType enum defines whether application is newly
   * registered or existing and being switched over another transport
   */
  enum class ApplicationType {
    kNewApplication,
    kSwitchedApplicationHashOk,
    kSwitchedApplicationWrongHashId
  };

  /**
   * @brief Prepares and sends RegisterAppInterface response to mobile
   * considering application type
   * @param app_type Type of application
   * @param add_info - additional information to be sent to mobile app
   **/
  void SendRegisterAppInterfaceResponseToMobile(ApplicationType app_type,
                                                const std::string& add_info);

  smart_objects::SmartObjectSPtr GetLockScreenIconUrlNotification(
      const uint32_t connection_key, app_mngr::ApplicationSharedPtr app);

  /**
   * @brief SendChangeRegistration send ChangeRegistration on HMI
   * @param function_id interface specific ChangeRegistration
   * @param language language of registration
   * @param app_id application to change registration
   */
  void SendChangeRegistration(const hmi_apis::FunctionID::eType function_id,
                              const int32_t language,
                              const uint32_t app_id);

  /**
   * @brief SendChangeRegistrationOnHMI send required SendChangeRegistration
   * HMI
   * @param app application to change registration
   */
  void SendChangeRegistrationOnHMI(app_mngr::ApplicationConstSharedPtr app);

  /**
   * @brief Sends OnAppRegistered notification to HMI
   *
   *@param application_impl application with changed HMI status
   *
   **/
  void SendOnAppRegisteredNotificationToHMI(
      const app_mngr::Application& application_impl,
      bool need_restore_vr = false);
  /**
   * @brief Check new ID along with known mobile application ID
   *
   * return TRUE if ID is known already, otherwise - FALSE
   */
  bool IsApplicationWithSameAppIdRegistered();

  /**
   * @brief Check new application parameters (name, tts, vr) for
   * coincidence with already known parameters of registered applications
   *
   * return SUCCESS if there is no coincidence of app.name/TTS/VR synonyms,
   * otherwise appropriate error code returns
  */
  mobile_apis::Result::eType CheckCoincidence();

  /**
  * @brief Predicate for using with CheckCoincidence method to compare with VR
  * synonym SO
  *
  * return TRUE if there is coincidence of VR, otherwise FALSE
  */
  struct CoincidencePredicateVR {
    CoincidencePredicateVR(const custom_str::CustomString& newItem)
        : newItem_(newItem) {}

    bool operator()(const smart_objects::SmartObject& obj) {
      const custom_str::CustomString& vr_synonym = obj.asCustomString();
      return newItem_.CompareIgnoreCase(vr_synonym);
    }
    const custom_str::CustomString& newItem_;
  };

  /**
   * @brief Check request parameters against policy table data
   * @return SUCCESS if check ok, otherwise return appropriate error code
   */
  mobile_apis::Result::eType CheckWithPolicyData();

  /**
   * @brief Fill DeviceInfo struct with data from request, if any
   * @param device_info Struct for device params from request
   */
  void FillDeviceInfo(policy::DeviceInfo* device_info);

  /**
   * @brief Checks register app interface params(ttsName, ...).
   * When type is String there is a check on the contents \t\n \\t \\n
   * @return if register app interface contains \t\n \\t \\n return TRUE,
   * FALSE otherwise
   */
  bool IsWhiteSpaceExist();

  /**
   * @brief Checks vehicle type params (model, year etc.) and in case of absense
   * replaces with policy table backup values
   * @param vehicle_type VehicleType struct
   * @param param Vehicle param
   * @param backup_value Backup value
   */
  void CheckResponseVehicleTypeParam(smart_objects::SmartObject& vehicle_type,
                                     const std::string& param,
                                     const std::string& backup_value);
  /**
   * @brief Sends ButtonSubscription notification at start up
   * to notify HMI that app subscribed on the custom button by default.
   */
  void SendSubscribeCustomButtonNotification();

  /**
   * @brief IsApplicationSwitched checks whether application is switched from
   * another transport. If application id is found, but not in reconnection
   * list, returns 'already registered' code. Otherwise - proceed with
   * switching.
   * @return True if application is detected as switched, otherwise false.
   */
  bool IsApplicationSwitched();

  /**
   * @brief TransformPolicyAppIdToUpper make policy app_id  in msg_params
   * uppercase.
   * SDL compare policy appid in not case sensitive mode.
   */
  void TransformPolicyAppIdToUpper();

  /**
   * @brief WaitForHMIIsReady blocking function. Waits for HMI be ready for
   * requests processing
   */
  void WaitForHMIIsReady();

  /**
   * @brief IsApplicationForbidden check if application is in list of forbidden
   * applications
   * @return true if application os not allowed to register, othervise return
   * false.
   */
  bool IsApplicationForbidden() const;

  /**
   * @brief ProcessApplicationTransportSwitching process checking if application
   * is switching transport.
   * If applicaiton is in switching mode, function will send response to mobile
   * If no switching required response wont be send
   * @return true if response to mobile was sent, otherwise return false.
   */
  bool ProcessApplicationTransportSwitching();

  /**
   * @brief IncrementDuplicateNameCounter increments internal counter of
   * duplicate name during registration
   * @param coincidence_result result of name checking
   */
  void IncrementDuplicateNameCounter(
      mobile_apis::Result::eType coincidence_result);

  /**
   * @brief SetupAppDeviceInfo add applicaiton device information to policies
   * @param application applicaiton to process
   */
  void SetupAppDeviceInfo(
      application_manager::ApplicationSharedPtr application);

  /**
   * @brief The DataResumeResult enum relust of resumption data check
   * RESUME_DATA : applicaiton data should be resumed
   * NO_HASH : hash_id is ommited in RAI. No data resumption required
   * WRONG_HASH : hash_id frim RAI does not match to savedhash id, resumption
   * should be failed.
   * MISSED_DATA : Some data was missed on SDL side.
   */
  enum class DataResumeResult { RESUME_DATA, NO_HASH, WRONG_HASH, MISSED_DATA };

  /**
   * @brief ApplicationDataShouldBeResumed check if application data should be
   * resumed
   * @return result of possible conditions for resumptions
   */
  DataResumeResult ApplicationDataShouldBeResumed();

  /**
   * @brief CheckLanguage check if language in RAI matches hmi_capabilities
   * Setup result_code variable in case of does not match
   */
  void CheckLanguage();

  application_manager::ApplicationSharedPtr application_;
  bool is_data_resumption_;
  std::string response_info_;
  mobile_apis::Result::eType result_code_;
  DISALLOW_COPY_AND_ASSIGN(RegisterAppInterfaceRequest);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_
