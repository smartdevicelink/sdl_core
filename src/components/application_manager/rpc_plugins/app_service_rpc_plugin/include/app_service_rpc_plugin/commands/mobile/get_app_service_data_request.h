/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_APP_SERVICE_RPC_PLUGIN_INCLUDE_APP_SERVICE_RPC_PLUGIN_COMMANDS_MOBILE_GET_APP_SERVICE_DATA_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_APP_SERVICE_RPC_PLUGIN_INCLUDE_APP_SERVICE_RPC_PLUGIN_COMMANDS_MOBILE_GET_APP_SERVICE_DATA_REQUEST_H_

#include "app_service_rpc_plugin/app_service_rpc_plugin.h"
#include "application_manager/commands/request_from_mobile_impl.h"

namespace app_service_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief GetAppServiceDataRequest command class
 **/
class GetAppServiceDataRequest
    : public app_mngr::commands::RequestFromMobileImpl {
 public:
  /**
   * @brief GetAppServiceDataRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  GetAppServiceDataRequest(const app_mngr::commands::MessageSharedPtr& message,
                           app_mngr::ApplicationManager& application_manager,
                           app_mngr::rpc_service::RPCService& rpc_service,
                           app_mngr::HMICapabilities& hmi_capabilities,
                           policy::PolicyHandlerInterface& policy_handle);

  /**
   * @brief GetAppServiceDataRequest class destructor
   **/
  virtual ~GetAppServiceDataRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Init sets hash update mode for request
   **/
  bool Init() FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  virtual void on_event(const app_mngr::event_engine::Event& event);

  virtual void on_event(const app_mngr::event_engine::MobileEvent& event);

 private:
  void HandleSubscribe();

  DISALLOW_COPY_AND_ASSIGN(GetAppServiceDataRequest);
};

}  // namespace commands

}  // namespace app_service_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_APP_SERVICE_RPC_PLUGIN_INCLUDE_APP_SERVICE_RPC_PLUGIN_COMMANDS_MOBILE_GET_APP_SERVICE_DATA_REQUEST_H_
