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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_VEHICLE_DATA_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_VEHICLE_DATA_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/application.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief SubscribeVehicleDataRequest command class
 **/
class SubscribeVehicleDataRequest : public CommandRequestImpl {
 public:
  /**
   * @brief SubscribeVehicleDataRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  explicit SubscribeVehicleDataRequest(const MessageSharedPtr& message);

  /**
   * @brief SubscribeButtonCommandRequest class destructor
   **/
  virtual ~SubscribeVehicleDataRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  virtual void on_event(const event_engine::Event& event);

#ifdef HMI_DBUS_API
 private:
  struct HmiRequest {
    hmi_apis::Common_Result::eType status;
    bool complete;
    smart_objects::SmartObject value;
    const char* str;
    hmi_apis::FunctionID::eType func_id;
  };

  typedef std::vector<HmiRequest> HmiRequests;
  HmiRequests hmi_requests_;
#endif // #ifdef HMI_DBUS_API

 private:
  /**
   * @brief Checks, if any app is subscribed for particular VI parameter
   * @param param_id VI parameter id
   * @return true, if there are registered apps subscribed for VI parameter,
   * otherwise - false
   */
  bool IsSomeoneSubscribedFor(const uint32_t param_id) const;

  /**
   * @brief Adds VI parameters being subscribed by another or the same app to
   * response with appropriate results
   * @param msg_params 'message_params' response section reference
   */
  void AddAlreadySubscribedVI(smart_objects::SmartObject& msg_params) const;

  /**
   * @brief Removes subscription for VI parameters which subsription attempt
   * returned an error
   * @param app Pointer to application sent subscribe request
   * @param msg_params 'message_parameters' response section reference
   */
  void UnsubscribeFailedSubscriptions(
      ApplicationSharedPtr app,
      const smart_objects::SmartObject& msg_params) const;

  /**
   * @brief VI parameters which had been already subscribed by another apps
   * befor particular app subscribed for these parameters
   */
  VehicleInfoSubscriptions vi_already_subscribed_by_another_apps_;

  /**
   * @brief VI parameters which had been subscribed already by particular app
   */
  VehicleInfoSubscriptions vi_already_subscribed_by_this_app_;

  DISALLOW_COPY_AND_ASSIGN(SubscribeVehicleDataRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_VEHICLE_DATA_REQUEST_H_
