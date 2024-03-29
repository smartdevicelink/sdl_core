/*

 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Copyright (c) 2018, Livio, Inc.
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SUBSCRIBE_VEHICLE_DATA_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SUBSCRIBE_VEHICLE_DATA_REQUEST_H_

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
 * @brief SubscribeVehicleDataRequest command class
 **/
class SubscribeVehicleDataRequest
    : public app_mngr::commands::RequestFromMobileImpl {
 public:
  /**
   * @brief SubscribeVehicleDataRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  SubscribeVehicleDataRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      VehicleInfoCommandParams params);

  /**
   * @brief SubscribeButtonCommandRequest class destructor
   **/
  ~SubscribeVehicleDataRequest();

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
   * @brief Adds VI parameters being subscribed by another or the same app to
   * response with appropriate results
   * @param msg_params 'message_params' response section reference
   */
  void AddAlreadySubscribedVI(smart_objects::SmartObject& msg_params) const;

  /**
   * @brief Actual subscription to pending vehicle data after successful
   * response from HMI
   * @param app Pointer to application sent subscribe request
   * @param msg_params 'message_parameters' response section reference
   */
  bool SubscribePendingVehicleData(app_mngr::ApplicationSharedPtr app,
                                   smart_objects::SmartObject& msg_params);

  /**
   * @brief Checks if current application and other applications
   * were subscribed to VI, prepare data that need to send to mobile app
   * or HMI.
   * @param app contains application
   * @param out_info contains resulting info for sending to mobile app
   * @param out_result_code contains result code for sending to mobile app
   * @param out_response_params contains parameters that SDL sends to
   * mobile application
   * @param out_request_params contains parameters that SDL sends to
   * HMI
   * @param result contains result that SDL sends to mobile app.
   */
  void CheckVISubscriptions(app_mngr::ApplicationSharedPtr app,
                            std::string& out_info,
                            mobile_apis::Result::eType& out_result_code,
                            smart_objects::SmartObject& out_response_params,
                            smart_objects::SmartObject& out_request_params,
                            bool& out_result);

  /**
   * @brief ConvertResponseToRequestName convert RPCSpec vehicle data names
   * from response naming to request naming.
   * This is workaround for cluster mode.
   * Parameter named in request message as `cluster_mode` and in response
   * message as `cluster_mode_status`
   * @param name mobile RPCSpec vehicle data name
   * @return hmi RPCSpec vehicle data name
   */
  const std::string& ConvertResponseToRequestName(const std::string& name);

  /**
   * @brief ConvertRequestToResponseName convert RPCSpec vehicle data names from
   * request to response
   * * This is workaround for cluster mode.
   * Parameter named in request message as `cluster_mode` and in response
   * message as `cluster_mode_status`
   * @param name mobile RPCSpec vehicle data name
   * @return hmi RPCSpec vehicle data name
   */
  const std::string& ConvertRequestToResponseName(const std::string& name);

  /**
   * @brief Appends data types for vehicle data in response to mobile
   * @param msg_params 'message_parameters' response section reference
   */
  void AppendDataTypesToMobileResponse(
      smart_objects::SmartObject& msg_params) const;

  /**
   * @brief Checks subscription status of certain vehicle_item
   * @param vi_name name of vehicle item to be checked
   * @param msg_params 'message_parameters' response section reference
   */
  bool CheckSubscriptionStatus(std::string vi_name,
                               const smart_objects::SmartObject& msg_params);

  /**
   * @brief VI parameters which had been already subscribed by another apps
   * befor particular app subscribed for these parameters
   */
  VehicleInfoSubscriptions vi_already_subscribed_by_another_apps_;

  /**
   * @brief VI parameters which had been subscribed already by particular app
   */
  VehicleInfoSubscriptions vi_already_subscribed_by_this_app_;

  /**
   * @brief VI parameters which wait for subscribe after HMI respond
   */
  VehicleInfoSubscriptions vi_waiting_for_subscribe_;

  CustomVehicleDataManager& custom_vehicle_data_manager_;

  DISALLOW_COPY_AND_ASSIGN(SubscribeVehicleDataRequest);
};

}  // namespace commands
}  // namespace vehicle_info_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SUBSCRIBE_VEHICLE_DATA_REQUEST_H_
