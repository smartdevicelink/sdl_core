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
#include "interfaces/MOBILE_API.h"
#include "application_manager/application_impl.h"
#include "utils/macro.h"
#include "connection_handler/device.h"

namespace application_manager {

namespace mobile_api = mobile_apis;

/*
 * @brief Typedef for HMI TextFieldName type
 */
typedef enum {
  MAIN_FILED1              = 0,
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
  GPS                      = 0,
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
    static void SendHMIStatusNotification(
      const ApplicationImpl& application_impl);

    /**
     * @brief Sends OnDeviceListUpdated notification to HMI
     *
     *@param device_list Device list
     *
     **/
    static void SendDeviceListUpdatedNotificationToHMI(
      const std::set<connection_handler::Device>& devices);

    /**
     * @brief Sends OnAppRegistered notification to HMI
     *
     *@param application_impl application with changed HMI status
     *
     **/
    static void SendOnAppRegisteredNotificationToHMI(
      const ApplicationImpl& application_impl);

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

    /*
     * @brief Prepare GetDeviceListResponse
     *
     *
     * @param devices Devices list
     *
     */
    static smart_objects::SmartObject* CreateDeviceListSO(
      const connection_handler::DeviceList& devices);

  private:
    MessageHelper();

    static const VehicleData      vehicle_data_;
    DISALLOW_COPY_AND_ASSIGN(MessageHelper);
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_MESSAGE_HELPER_H_
