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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_APP_EXTENSION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_APP_EXTENSION_H

#include <application_manager/application_manager.h>

#include <set>
#include <string>

namespace vehicle_info_plugin {
namespace app_mngr = application_manager;

typedef mobile_apis::VehicleDataType::eType VehicleDataType;
/**
 * @brief Defines set of vehicle info types
 */
typedef std::set<mobile_apis::VehicleDataType::eType> VehicleInfoSubscriptions;

class VehicleInfoAppExtension : app_mngr::AppExtension {
 public:
  explicit VehicleInfoAppExtension(app_mngr::AppExtensionUID uid);
  virtual ~VehicleInfoAppExtension();

  void subscribeToVehicleInfo(const VehicleDataType vehicle_data);
  void unsubscribeFromVehicleInfo(const VehicleDataType vehicle_data);
  void unsubscribeFromVehicleInfo();
  bool isSubscribedToVehicleInfo(const VehicleDataType vehicle_data_type) const;
  VehicleInfoSubscriptions Subscriptions();

  void SaveResumptionData(
      NsSmartDeviceLink::NsSmartObjects::SmartObject& resumption_data) OVERRIDE;
  void PorcessResumption(const smart_objects::SmartObject& resumption_data) OVERRIDE;

 private:
  VehicleInfoSubscriptions subscribed_data_;
};
}

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_APP_EXTENSION_H
