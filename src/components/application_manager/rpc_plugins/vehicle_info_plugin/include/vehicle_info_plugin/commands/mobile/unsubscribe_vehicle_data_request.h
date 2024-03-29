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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_UNSUBSCRIBE_VEHICLE_DATA_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_UNSUBSCRIBE_VEHICLE_DATA_REQUEST_H_

#include "application_manager/application.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/request_from_mobile_impl.h"
#include "utils/macro.h"
#include "vehicle_info_plugin/custom_vehicle_data_manager.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"
#include "vehicle_info_plugin/vehicle_info_command_params.h"

namespace vehicle_info_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief UnsubscribeVehicleDataRequest command class
 **/
class UnsubscribeVehicleDataRequest
    : public app_mngr::commands::RequestFromMobileImpl {
 public:
  /**
   * @brief UnsubscribeVehicleDataRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  UnsubscribeVehicleDataRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      const VehicleInfoCommandParams& params);

  /**
   * @brief UnsubscribeVehicleDataRequest class destructor
   **/
  ~UnsubscribeVehicleDataRequest();

  /**
   * @brief Execute command
   **/
  void Run() FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  /**
   * @brief Init sets hash update mode for request
   */
  bool Init() FINAL;

 private:
  /**
   * @brief Checks, if any app is subscribed for particular VI parameter
   * @param param_id VI parameter id
   * @return true, if there are registered apps subscribed for VI parameter,
   * otherwise - false
   */
  bool IsSomeoneSubscribedFor(const std::string& param_name) const;

  /**
   * @brief Adds VI parameters being unsubscribed by another or the same app to
   * response with appropriate results
   * @param msg_params 'message_params' response section reference
   */
  void AddAlreadyUnsubscribedVI(smart_objects::SmartObject& response) const;

  /**
   * @brief Actual unsubscription from all pending vehicle data. To be called
   * after successful HMI response
   * @param app shared pointer to application, which initialized unsubscription
   * @param msg_params 'message_params' response section reference
   */
  bool UnsubscribePendingVehicleData(
      app_mngr::ApplicationSharedPtr app,
      const smart_objects::SmartObject& msg_params);

  /**
   * @brief ConvertRequestToResponseName convert RPCSpec vehicle data names
   * from hmi api to mobile api
   * @param name mobile RPCSpec vehicle data name
   * @return hmi RPCSpec vehicle data name
   */
  const std::string& ConvertRequestToResponseName(const std::string& name);

  /**
   * @brief ConvertResponseToRequestName convert RPCSpec vehicle data names
   * from hmi api to mobile api
   * @param name mobile RPCSpec vehicle data name
   * @return hmi RPCSpec vehicle data name
   */
  const std::string& ConvertResponseToRequestName(const std::string& name);

  /**
   * @brief Appends data types for vehicle data in response to mobile
   * @param msg_params 'message_parameters' response section reference
   */
  void AppendDataTypesToMobileResponse(
      smart_objects::SmartObject& msg_params) const;

  /**
   * @brief Checks subscription status of certain vehicle_item
   * @param key name of vehicle item to be checked
   * @param msg_params 'message_parameters' response section reference
   */
  bool CheckSubscriptionStatus(std::string key,
                               const smart_objects::SmartObject& msg_params);

  smart_objects::SmartObject response_params_;

  /**
   * @brief VI parameters which still being subscribed by another apps after
   * particular app had been unsubscribed from these parameters
   */
  VehicleInfoSubscriptions vi_still_subscribed_by_another_apps_;

  /**
   * @brief VI parameters which had been unsubscribed already by particular
   * app
   */
  VehicleInfoSubscriptions vi_already_unsubscribed_by_this_app_;

  /**
   * @brief VI parameters to be unsubscribed after HMI response
   */
  VehicleInfoSubscriptions vi_waiting_for_unsubscribe_;

  CustomVehicleDataManager& custom_vehicle_data_manager_;

  DISALLOW_COPY_AND_ASSIGN(UnsubscribeVehicleDataRequest);
};

}  // namespace commands
}  // namespace vehicle_info_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_UNSUBSCRIBE_VEHICLE_DATA_REQUEST_H_
