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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_HELPER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_HELPER_H_

#include <map>
#include <string>
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "utils/macro.h"
#include "connection_handler/device.h"
#include "application_manager/application.h"
#include "application_manager/vehicle_info_data.h"
#include "policy/policy_types.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
}
}

namespace application_manager {
namespace mobile_api = mobile_apis;
namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

/*
 * @brief Typedef for HMI TextFieldName type
 */
typedef enum {
  MAIN_FILED1 = 0,
  MAIN_FILED2,
  MAIN_FILED3,
  MAIN_FILED4,
  STATUS_BAR,
  MEDIA_CLOCK,
  MEDIA_TRACK,
  ALERT_TEXT1,
  ALERT_TEXT2,
  ALERT_TEXT3,
  SCROLLABLE_MSG_BODY,
  INITIAL_INTERACTION_TEXT,
  NAVI_TEXT1,
  NAVI_TEXT2,
  ETA,
  TOTAL_DISTANCE,
  NAVI_TEXT,
  AUDIO_DISPLAY_TEXT1,
  AUDIO_DISPLAY_TEXT2,
  SLIDER_HADER,
  SLIDER_FOOTEER
} TextFieldName;

/*
 * @brief Typedef for VehicleData
 *
 * @param const char* Name of the parameter in mobile request
 * @param VehicleDataType Enum for vehicle data
 */
typedef std::map<const char*, VehicleDataType> VehicleData;

/**
 * @brief MessageHelper class
 **/
class MessageHelper {
  public:

    typedef std::vector<smart_objects::SmartObject*> SmartObjectList;

    /**
     * @brief Sends HMI status notification to mobile
     *
     *@param application_impl application with changed HMI status
     *
     **/
    static void SendHMIStatusNotification(const Application& application_impl);

    /**
     * @brief Sends OnAppRegistered notification to HMI
     *
     *@param application_impl application with changed HMI status
     *
     **/
    static void SendOnAppRegisteredNotificationToHMI(
      const Application& application_impl, bool resumption = false);

    /**
     * @brief Create mobile HashUpdateNotification
     */
    static smart_objects::SmartObject* GetHashUpdateNotification(const uint32_t app_id);

    /**
     * @brief Sends to mobile HashUpdateNotification
     */
    static void SendHashUpdateNotification(const uint32_t app_id);

    /**
     * @brief Sends OnAppInterfaceUnregistered notification to mobile
     *
     *@param connection_key Connection key
     *@param reason Reason
     *
     **/
    static void SendOnAppInterfaceUnregisteredNotificationToMobile(
      int32_t connection_key,
      mobile_apis::AppInterfaceUnregisteredReason::eType reason);

    /*
     * @brief Retrieve vehicle data map for param name in mobile request
     * to VehicleDataType
     *
     * @return VehicleData reference
     */
    static const VehicleData& vehicle_data();

    /**
     * @brief Convert string to HMI level, if possible
     * @param hmi_level Stringified HMI level
     * @return Appropriate enum from HMI level, or INVALID_ENUM, if conversiion
     * is not possible
     */
    static mobile_api::HMILevel::eType StringToHMILevel(
        const std::string& hmi_level);

    /*
    * @brief Used to obtain string representation of app's
    * HMI Level.
    * @param hmi_level Desired HMI Level
    */
    static const char* StringifiedHMILevel(
      mobile_apis::HMILevel::eType hmi_level);

    /*
    * @brief Used to obtain function name by its id
    * @param function_id Function ID
    */
    static std::string StringifiedFunctionID(
      mobile_apis::FunctionID::eType function_id);

    static void CreateGetDeviceData(int32_t correlation_id);

    static smart_objects::SmartObject* CreateBlockedByPoliciesResponse(
      mobile_apis::FunctionID::eType function_id,
      mobile_apis::Result::eType result, uint32_t correlation_id,
      uint32_t connection_key);

    /*
     * @brief Prepare GetDeviceListResponse
     *
     *
     * @param devices Devices list
     *
     */
    static smart_objects::SmartObject* CreateDeviceListSO(
      const connection_handler::DeviceList& devices);

    static smart_objects::SmartObject* CreateModuleInfoSO(
      uint32_t function_id);

    static smart_objects::SmartObject* CreateSetAppIcon(
      const std::string& path_to_icon, uint32_t app_id);

    /**
     * @brief Sends IVI subscriptions
     */
    static bool SendIVISubscribtions(const uint32_t app_id);

    /**
     * @brief Sends IVI subscriptions
     */
    static SmartObjectList GetIVISubscribtionRequests(const uint32_t app_id);

    static void SendAppDataToHMI(ApplicationConstSharedPtr app);
    static void SendGlobalPropertiesToHMI(ApplicationConstSharedPtr app);
    static SmartObjectList CreateGlobalPropertiesRequestsToHMI(ApplicationConstSharedPtr app);

    static smart_objects::SmartObject* CreateAppVrHelp(ApplicationConstSharedPtr app);

    static SmartObjectList CreateShowRequestToHMI(ApplicationConstSharedPtr app);
    static void SendShowRequestToHMI(ApplicationConstSharedPtr app);
    static void SendShowConstantTBTRequestToHMI(ApplicationConstSharedPtr app);
    static void SendAddCommandRequestToHMI(ApplicationConstSharedPtr app);
    static SmartObjectList CreateAddCommandRequestToHMI(ApplicationConstSharedPtr app);

    static void SendChangeRegistrationRequestToHMI(ApplicationConstSharedPtr app);
    static void SendAddVRCommandToHMI(
      uint32_t cmd_id, const smart_objects::SmartObject& vr_commands,
      uint32_t app_id);

    static smart_objects::SmartObject* CreateAddVRCommandToHMI(
      uint32_t cmd_id, const smart_objects::SmartObject& vr_commands,
      uint32_t app_id);

    static void SendAddSubMenuRequestToHMI(ApplicationConstSharedPtr app);
    static SmartObjectList CreateAddSubMenuRequestToHMI(ApplicationConstSharedPtr app);

    static void SendOnSdlCloseNotificationToHMI();
    static void SendOnAppUnregNotificationToHMI(ApplicationConstSharedPtr app);
    static void ResetGlobalproperties(ApplicationSharedPtr app);

    static void SendActivateAppToHMI(uint32_t const app_id);
    static void GetDeviceInfoForHandle(const uint32_t device_handle,
                                       policy::DeviceParams* device_info);
    static void GetDeviceInfoForApp(uint32_t connection_key,
                                    policy::DeviceParams* device_info);

    /**
    * @brief Send SDL_ActivateApp response to HMI
    * @param permissions response parameters
    */
    static void SendActivateAppResponse(policy::AppPermissions& permissions,
                                        uint32_t correlation_id);

    /**
    * @brief Send OnSDLConsentNeeded to HMI for device data consent by user
    * @param device_info Device info, e.g. mac, handle, name
    */
    static void SendOnSDLConsentNeeded(const policy::DeviceParams& device_info);

    /**
      * @brief Send request to SyncP process to read file and send
      * Policy Table Snapshot using Retry Strategy
      * @param file_path Path to file with PTS
      * @param timeout Timeout to wait for PTU
      * @param retries Seconds between retries
      */
    static void SendPolicyUpdate(const std::string& file_path,
                                 int timeout,
                                 const std::vector<int>& retries);

    /**
     * @brief Send GetUserFriendlyMessage response to HMI
     * @param msg Appopriate messages params
     * @param correlation_id Correlation id of request
     */
    static void SendGetUserFriendlyMessageResponse(
      const std::vector<policy::UserFriendlyMessage>& msg,
      uint32_t correlation_id);

    /**
     * @brief Send GetListOfPermissions response to HMI
     * @param permissions Array of groups permissions
     * @param correlation_id Correlation id of request
     */
    static void SendGetListOfPermissionsResponse(
      std::vector<policy::FunctionalGroupPermission>& permissions,
      uint32_t correlation_id);

    /*
     * @brief Sends notification to HMI to start video streaming
     *
     * @param url             URL for video streamng
     * @param connection_key  Application connection key
     *
     */
    static void SendNaviStartStream(const std::string& url, int32_t connection_key);

    /*
     * @brief Sends notification to HMI to stop video streaming
     *
     * @param connection_key  Application connection key
     *
     */
    static void SendNaviStopStream(int32_t connection_key);

    /*
    * @brief Send notification for Update of Policy Table
    * with PT Snapshot.
    * @param connection_key Id of application to send message to
    * @param policy_data PT Snapshot
    * @param url If empty string, no URL is provided
    * @param timeout If -1 no timeout is provdied
    */
    static void SendPolicySnapshotNotification(
      unsigned int connection_key, const std::vector<uint8_t>& policy_data,
      const std::string& url = "", int timeout = -1);

    /*
    * @brief Send notification to mobile on application permissions update
    * @param connection_key Id of application to send message to
    * @param permissions updated permissions for application
    */
    static void SendOnPermissionsChangeNotification(
      uint32_t connection_key, const policy::Permissions& permissions);

    /*
    * @brief Send notification to HMI on application permissions update
    * @param connection_key Id of application to send message to
    * @param permissions updated permissions for application
    */
    static void SendOnAppPermissionsChangedNotification(
      uint32_t connection_key, const policy::AppPermissions& permissions);

    /**
     * @brief Send GetStatusUpdate response to HMI with current policy update
     * status
     * @param status Update status
     * @param correlation_id Correlation id from request
     */
    static void SendGetStatusUpdateResponse(const std::string& status,
                                            uint32_t correlation_id);

    /**
     * @brief Send UpdateSDL response to HMI with policy update result
     * @param result Update result
     * @param correlation_id Correlation id from request
     */
    static void SendUpdateSDLResponse(const std::string& result,
                                            uint32_t correlation_id);

    /**
     * @brief Send OnStatusUpdate to HMI on policy update status change
     * @param status Policy table update status
     */
    static void SendOnStatusUpdate(const std::string& status);

    /**
     * @brief Send GetSystemInfo request to HMI
     */
    static void SendGetSystemInfoRequest();

    /*
     * @brief Sends notification to HMI to start audio streaming
     *
     * @param url             URL for video streamng
     * @param connection_key  Application connection key
     *
     */
    static void SendAudioStartStream(const std::string& url, int32_t connection_key);

    /*
     * @brief Sends notification to HMI to stop audio streaming
     *
     * @param connection_key  Application connection key
     *
     */
    static void SendAudioStopStream(int32_t connection_key);

    /*
     * @brief Sends notification to HMI to stop audioPathThru
     *
     * @param connection_key  Application connection key
     *
     * @return TRUE on SUCCES otherwise return FALSE
     */
    static bool SendStopAudioPathThru();

    static smart_objects::SmartObject* CreateNegativeResponse(
      uint32_t connection_key, int32_t function_id, uint32_t correlation_id,
      int32_t result_code);

    /*
     * @brief Verify image and add image file full path
     *
     * @param SmartObject with image
     *
     * @param app current application
     *
     * @return verification result
     *
     */
    static mobile_apis::Result::eType VerifyImage(smart_objects::SmartObject& image,
        ApplicationConstSharedPtr app);

    /*
     * @brief Finds "Image" structure in request and verify image file presence
     *                      in Core.
     *
     * @param message SmartObject with request
     *
     * @param app current application
     *
     * @return verification result
     *
     */
    static mobile_apis::Result::eType VerifyImageFiles(
      smart_objects::SmartObject& message, ApplicationConstSharedPtr app);

    static bool VerifySoftButtonText(smart_objects::SmartObject& soft_button);

    static mobile_apis::Result::eType ProcessSoftButtons(
      smart_objects::SmartObject& message_params,
      ApplicationConstSharedPtr app);

    static bool PrintSmartObject(const smart_objects::SmartObject& object);

    template<typename From, typename To>
    static To ConvertEnumAPINoCheck(const From& input) {
      return static_cast<To>(input);
    }

    /**
     * @brief Convert common language to string representation
     * @param language Common language
     * @return Common language string representation
     */
    static std::string CommonLanguageToString(
        hmi_apis::Common_Language::eType language);
private:
    static smart_objects::SmartObject* CreateChangeRegistration(
      int32_t function_id, int32_t language, uint32_t app_id);

    MessageHelper();

    static const VehicleData vehicle_data_;
    DISALLOW_COPY_AND_ASSIGN(MessageHelper);
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_HELPER_H_
