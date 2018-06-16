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

#include "vehicle_info_plugin/vehicle_info_plugin.h"
#include "vehicle_info_plugin/vehicle_info_command_factory.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/message_helper.h"

namespace vehicle_info_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPlugin")

namespace strings = application_manager::strings;

std::pair<std::string,
          mobile_apis::VehicleDataType::eType> kVehicleDataInitializer[] = {
    std::make_pair(strings::gps, mobile_apis::VehicleDataType::VEHICLEDATA_GPS),
    std::make_pair(strings::speed,
                   mobile_apis::VehicleDataType::VEHICLEDATA_SPEED),
    std::make_pair(strings::rpm, mobile_apis::VehicleDataType::VEHICLEDATA_RPM),
    std::make_pair(strings::fuel_level,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELLEVEL),
    std::make_pair(strings::fuel_level_state,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELLEVEL_STATE),
    std::make_pair(strings::instant_fuel_consumption,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELCONSUMPTION),
    std::make_pair(strings::fuel_range,
                   mobile_apis::VehicleDataType::VEHICLEDATA_FUELRANGE),
    std::make_pair(strings::external_temp,
                   mobile_apis::VehicleDataType::VEHICLEDATA_EXTERNTEMP),
    std::make_pair(strings::vin, mobile_apis::VehicleDataType::VEHICLEDATA_VIN),
    std::make_pair(strings::prndl,
                   mobile_apis::VehicleDataType::VEHICLEDATA_PRNDL),
    std::make_pair(strings::tire_pressure,
                   mobile_apis::VehicleDataType::VEHICLEDATA_TIREPRESSURE),
    std::make_pair(strings::odometer,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ODOMETER),
    std::make_pair(strings::belt_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_BELTSTATUS),
    std::make_pair(strings::body_information,
                   mobile_apis::VehicleDataType::VEHICLEDATA_BODYINFO),
    std::make_pair(strings::device_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_DEVICESTATUS),
    std::make_pair(strings::driver_braking,
                   mobile_apis::VehicleDataType::VEHICLEDATA_BRAKING),
    std::make_pair(strings::wiper_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_WIPERSTATUS),
    std::make_pair(strings::head_lamp_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_HEADLAMPSTATUS),
    std::make_pair(strings::e_call_info,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ECALLINFO),
    std::make_pair(strings::airbag_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_AIRBAGSTATUS),
    std::make_pair(strings::emergency_event,
                   mobile_apis::VehicleDataType::VEHICLEDATA_EMERGENCYEVENT),
    std::make_pair(strings::cluster_mode_status,
                   mobile_apis::VehicleDataType::VEHICLEDATA_CLUSTERMODESTATUS),
    std::make_pair(strings::my_key,
                   mobile_apis::VehicleDataType::VEHICLEDATA_MYKEY),
    /*
     NOT DEFINED in mobile APIVehicleInfoPlugin
     std::make_pair(strings::gps,
     BATTVOLTAGE),
     */
    std::make_pair(strings::engine_torque,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ENGINETORQUE),
    std::make_pair(strings::acc_pedal_pos,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ACCPEDAL),
    std::make_pair(strings::steering_wheel_angle,
                   mobile_apis::VehicleDataType::VEHICLEDATA_STEERINGWHEEL),
    std::make_pair(strings::engine_oil_life,
                   mobile_apis::VehicleDataType::VEHICLEDATA_ENGINEOILLIFE)};

const VehicleInfoPlugin::VehicleData VehicleInfoPlugin::vehicle_data_(
    kVehicleDataInitializer,
    kVehicleDataInitializer + ARRAYSIZE(kVehicleDataInitializer));

VehicleInfoPlugin::VehicleInfoPlugin() : application_manager_(nullptr) {}

bool VehicleInfoPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler) {
  application_manager_ = &app_manager;
  command_factory_.reset(new vehicle_info_plugin::VehicleInfoCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  return true;
}

bool VehicleInfoPlugin::IsAbleToProcess(
    const int32_t function_id, const commands::Command::CommandSource source) {
  return command_factory_->IsAbleToProcess(function_id, source);
}

std::string VehicleInfoPlugin::PluginName() {
  return "Vehicle Info Plugin";
}

app_mngr::CommandFactory& VehicleInfoPlugin::GetCommandFactory() {
  return *command_factory_;
}

void VehicleInfoPlugin::OnPolicyEvent(plugins::PolicyEvent event) {}

void VehicleInfoPlugin::OnApplicationEvent(
    plugins::ApplicationEvent event,
    app_mngr::ApplicationSharedPtr application) {}

void VehicleInfoPlugin::ProcessResumptionSubscription(
    application_manager::ApplicationSharedPtr app,
    VehicleInfoAppExtension& ext) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  const auto& subscriptions = ext.Subscriptions();
  for (auto& ivi_data : vehicle_data_) {
    mobile_apis::VehicleDataType::eType type_id = ivi_data.second;
    if (subscriptions.end() != subscriptions.find(type_id)) {
      std::string key_name = ivi_data.first;
      msg_params[key_name] = true;
    }
  }
  smart_objects::SmartObjectSPtr request =
      application_manager::MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData,
          *application_manager_);
  (*request)[strings::msg_params] = msg_params;
  application_manager_->GetRPCService().ManageHMICommand(request);
}

auto FindAppSubscribedToIVI(mobile_apis::VehicleDataType::eType ivi_data,
                            application_manager::ApplicationManager& app_mngr) {
  auto& applications = app_mngr.applications();

  for (auto& app : applications) {
    auto& ext = VehicleInfoAppExtension::ExtractVIExtension(app);
    if (ext.isSubscribedToVehicleInfo(ivi_data)) {
      return app;
    }
  }
  return application_manager::ApplicationSharedPtr;
}

auto GetUnsubscribeIVIRequest(
    int32_t ivi_id, application_manager::ApplicationManager& app_mngr) {
  using namespace smart_objects;

  auto find_ivi_name = [ivi_id]() {
    for (auto item : VehicleInfoPlugin::vehicle_data_) {
      if (ivi_id == item.second) {
        return item.first;
      }
    }
    return std::string();
  };
  std::string key_name = find_ivi_name();
  DCHECK_OR_RETURN_VOID(!key_name.empty());
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[key_name] = true;

  SmartObjectSPtr message = CreateMessageForHMI(
      hmi_apis::messageType::request, app_mngr.GetNextHMICorrelationID());
  DCHECK(message);

  SmartObject& object = *message;
  object[strings::params][strings::function_id] =
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData;

  object[strings::msg_params] = msg_params;
  return message;
}

void VehicleInfoPlugin::DeleteSubscriptions(
    application_manager::ApplicationSharedPtr app) {
  auto& ext = VehicleInfoAppExtension::ExtractVIExtension(app);
  auto subscriptions = ext.Subscriptions();
  for (auto& ivi : subscriptions) {
    ext.unsubscribeFromVehicleInfo(ivi);
    still_subscribed_app = FindAppSubscribedToIVI(ivi, application_manager_);
    if (!still_subscribed_app) {
      auto message = GetUnsubscribeIVIRequest(ivi, application_manager_);
      app_mngr.GetRPCService().ManageHMICommand(message);
    }
  }
}
}

extern "C" application_manager::plugin_manager::RPCPlugin* Create() {
  return new vehicle_info_plugin::VehicleInfoPlugin();
}
