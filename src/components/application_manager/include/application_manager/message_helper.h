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
#include "application_manager/application.h"
#include "utils/macro.h"
#include "connection_handler/device.h"

namespace application_manager {

namespace mobile_api = mobile_apis;

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
 * @brief Typedef for the vehicle data types that can
 * be published and subscribed to
 */
typedef enum {
  GPS = 0,
  SPEED,
  RPM,
  FUELLEVEL,
  FUELLEVEL_STATE,
  FUELCONSUMPTION,
  EXTERNTEMP,
  VIN,
  PRNDL,
  TIREPRESSURE,
  ODOMETER,
  BELTSTATUS,
  BODYINFO,
  DEVICESTATUS,
  ECALLINFO,
  AIRBAGSTATUS,
  EMERGENCYEVENT,
  CLUSTERMODESTATUS,
  MYKEY,
  BRAKING,
  WIPERSTATUS,
  HEADLAMPSTATUS,
  BATTVOLTAGE,
  ENGINETORQUE,
  ACCPEDAL,
  STEERINGWHEEL
} VehicleDataType;

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
      const Application& application_impl);

    /**
     * @brief Sendss Vr Command 'Help'
     */
    static void SendHelpVrCommand();

    /**
     * @brief Send Vr Synonyms of application name to HMI
     * so that app can be activated via VR.
     */
    static void SendVrCommandsOnRegisterAppToHMI(Application* app);

    /**
     * @brief Removes Vr Synonyms of application name from HMI
     * when unregistering application.
     */
    static void SendRemoveVrCommandsOnUnregisterApp(Application* app);

    /**
     * @brief Sends OnAppInterfaceUnregistered notification to mobile
     *
     *@param connection_key Connection key
     *@param reason Reason
     *
     **/
    static void SendOnAppInterfaceUnregisteredNotificationToMobile(
      int connection_key,
      mobile_apis::AppInterfaceUnregisteredReason::eType reason);

    /*
     * @brief Retrieve vehicle data map for param name in mobile request
     * to VehicleDataType
     *
     * @return VehicleData reference
     */
    static const VehicleData& vehicle_data();

    static smart_objects::SmartObject* CreateBlockedByPoliciesResponse(
      mobile_apis::FunctionID::eType function_id,
      mobile_apis::Result::eType result, unsigned int correlation_id,
      unsigned int connection_key);

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
      unsigned int function_id);

    static smart_objects::SmartObject* CreateSetAppIcon(
      const std::string& path_to_icon, unsigned int app_id);

    static void SendAppDataToHMI(const Application* app);
    static void SendGlobalPropertiesToHMI(const Application* app);

    static smart_objects::SmartObject* CreateAppVrHelp(const Application* app);
    static void SendShowVrHelpToHMI(const Application* app);

    static void SendShowRequestToHMI(const Application* app);
    static void SendShowConstantTBTRequestToHMI(const Application* app);
    static void SendAddCommandRequestToHMI(const Application* app);
    static void SendChangeRegistrationRequestToHMI(const Application* app);
    static void SendAddVRCommandToHMI(
      unsigned int cmd_id, const smart_objects::SmartObject& vr_commands,
      unsigned int app_id);
    static void SendAddSubMenuRequestToHMI(const Application* app);
    static void RemoveAppDataFromHMI(Application* const app);
    static void SendOnAppUnregNotificationToHMI(Application* const app);
    static void SendDeleteCommandRequestToHMI(Application* const app);
    static void SendDeleteSubMenuRequestToHMI(Application* const app);
    static void ResetGlobalproperties(Application* const app);

    static void SendActivateAppToHMI(Application* const app);


    /*
     * @brief Sends notification to HMI to start video streaming
     *
     * @param url             URL for video streamng
     * @param connection_key  Application connection key
     *
     */
    static void SendNaviStartStream(const std::string& url, int connection_key);

    /*
     * @brief Sends notification to HMI to stop video streaming
     *
     * @param connection_key  Application connection key
     *
     */
    static void SendNaviStopStream(int connection_key);

    static smart_objects::SmartObject* CreateNegativeResponse(
      unsigned int connection_key, int function_id, unsigned int correlation_id,
      int result_code);

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
                            const Application* app);

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
      smart_objects::SmartObject& message, const Application* app);

    static bool VerifySoftButtonText(smart_objects::SmartObject& soft_button);

    static mobile_apis::Result::eType ProcessSoftButtons(
                                  smart_objects::SmartObject& message_params,
                                  const Application* app);

    /*
     * @brief Verify application and tts name in RefisterAppInterface request msg_params
     *
     * @param message msg_params
     *
     *
     * @return verification result
     *
     */
    static bool VerifyApplicationName(smart_objects::SmartObject& msg_params);

    static bool PrintSmartObject(smart_objects::SmartObject& object);

    template<typename From, typename To>
    static To ConvertEnumAPINoCheck(const From& input) {
      return static_cast<To>(input);
    }

  private:
    static smart_objects::SmartObject* CreateChangeRegistration(
      int function_id, int language, unsigned int app_id);
    static smart_objects::SmartObject* CreateGeneralVrCommand();
    static void SendRemoveCommandToHMI(int function_id, int command_id,
                                       unsigned int app_id);
    MessageHelper();

    static const VehicleData vehicle_data_;
    DISALLOW_COPY_AND_ASSIGN(MessageHelper);
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_HELPER_H_
