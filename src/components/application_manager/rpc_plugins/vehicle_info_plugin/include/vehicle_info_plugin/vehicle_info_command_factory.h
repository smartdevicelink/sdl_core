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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_COMMAND_FACTORY_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_COMMAND_FACTORY_H

#include "application_manager/application_manager.h"
#include "vehicle_info_plugin/custom_vehicle_data_manager.h"
#include "vehicle_info_plugin/vehicle_info_hmi_command_factory.h"
#include "vehicle_info_plugin/vehicle_info_mobile_command_factory.h"

namespace vehicle_info_plugin {

/**
 * @brief The Vehicle Info command factory.
 */
class VehicleInfoCommandFactory : public application_manager::CommandFactory {
 public:
  VehicleInfoCommandFactory(
      application_manager::ApplicationManager& application_manager,
      application_manager::rpc_service::RPCService& rpc_service,
      application_manager::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler,
      CustomVehicleDataManager& custom_vehicle_data_manager);
  virtual ~VehicleInfoCommandFactory();

  application_manager::CommandSharedPtr CreateCommand(
      const application_manager::commands::MessageSharedPtr& message,
      application_manager::commands::Command::CommandSource source) OVERRIDE;

  bool IsAbleToProcess(
      const int32_t function_id,
      const application_manager::commands::Command::CommandSource source)
      const OVERRIDE;

 private:
  std::unique_ptr<application_manager::CommandFactory> hmi_command_factory_;
  std::unique_ptr<application_manager::CommandFactory> mob_command_factory_;
};
}  // namespace vehicle_info_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_COMMAND_FACTORY_H
