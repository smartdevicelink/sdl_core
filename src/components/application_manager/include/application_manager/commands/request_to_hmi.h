/*
 Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_REQUEST_TO_HMI_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_REQUEST_TO_HMI_H_

#include "application_manager/commands/command_impl.h"
#include "application_manager/application_manager.h"

namespace application_manager {

namespace commands {

/**
 * @brief Check if HMI's interface is available.
 * @param application_manager contains application manager component
 * @param interface contains name of HMI's interface
 * @return true if interface is available otherwise return fasle.
 */
bool CheckAvailabilityHMIInterfaces(ApplicationManager& application_manager,
                                    HmiInterfaces::InterfaceID interface);

/**
 * @brief Change interface state
 * @param application_manager contains ApplicationManager instance
 * @param response_from_hmi contains response from HMI
 * @param interface contanins InterfaceID whose state is changed.
 * @return true if field available exist and contains true in response params
 * otherwise return false.
 */
bool ChangeInterfaceState(ApplicationManager& application_manager,
                          const smart_objects::SmartObject& response_from_hmi,
                          HmiInterfaces::InterfaceID interface);

class RequestToHMI : public CommandImpl {
 public:
  RequestToHMI(const MessageSharedPtr& message,
               ApplicationManager& application_manager);
  virtual ~RequestToHMI();
  virtual bool Init();
  virtual bool CleanUp();
  virtual void Run();
  void SendRequest();

  /*
   * @brief Retrieves application ID
   */
  inline uint32_t application_id() const {
    return (*message_)[strings::msg_params][strings::app_id].asUInt();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(RequestToHMI);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_HMI_REQUEST_TO_HMI_H_
