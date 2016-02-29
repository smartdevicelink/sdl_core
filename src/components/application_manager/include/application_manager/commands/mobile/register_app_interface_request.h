/*

 Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_

#include <string.h>
#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"
#include "utils/custom_string.h"

namespace policy {
struct DeviceInfo;
}

namespace application_manager {

class Application;

namespace commands {

namespace custom_str = utils::custom_string;

/**
 * @brief Register app interface request  command class
 **/
class RegisterAppInterfaceRequest : public CommandRequestImpl {
 public:
  /**
   * \brief RegisterAppInterfaceRequest class constructor
   **/
  explicit RegisterAppInterfaceRequest(const MessageSharedPtr& message);

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
   * @brief Sends RegisterAppInterface response to mobile
   *
   *@param application_impl application
   *
   **/
  void SendRegisterAppInterfaceResponseToMobile();

 private:
  /**
   * @brief Sends OnAppRegistered notification to HMI
   *
   *@param application_impl application with changed HMI status
   *
   **/
  void SendOnAppRegisteredNotificationToHMI(const Application& application_impl,
                                            bool resumption = false,
                                            bool need_restore_vr = false);
  /*
   * @brief Check new ID along with known mobile application ID
   *
   * return TRUE if ID is known already, otherwise - FALSE
   */
  bool IsApplicationWithSameAppIdRegistered();

  /*
   * @brief Check new application parameters (name, tts, vr) for
   * coincidence with already known parameters of registered applications
   *
   * return SUCCESS if there is no coincidence of app.name/TTS/VR synonyms,
   * otherwise appropriate error code returns
  */
  mobile_apis::Result::eType CheckCoincidence();

  /*
  * @brief Predicate for using with CheckCoincidence method to compare with VR
  * synonym SO
  *
  * return TRUE if there is coincidence of VR, otherwise FALSE
  */
  struct CoincidencePredicateVR {
    explicit CoincidencePredicateVR(const custom_str::CustomString& newItem)
        : newItem_(newItem){};

    bool operator()(const smart_objects::SmartObject& obj) {
      const custom_str::CustomString& vr_synonym = obj.asCustomString();
      return newItem_.CompareIgnoreCase(vr_synonym);
    };
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

 private:
  std::string response_info_;
  mobile_apis::Result::eType result_checking_app_hmi_type_;

  DISALLOW_COPY_AND_ASSIGN(RegisterAppInterfaceRequest);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_REGISTER_APP_INTERFACE_REQUEST_H_
