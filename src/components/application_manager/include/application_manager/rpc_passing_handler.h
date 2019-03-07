/*
 * Copyright (c) 2018, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_PASSING_HANDLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_PASSING_HANDLER_H_

#include "application_manager/application.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"
#include "application_manager/app_service_manager.h"
#include <deque>
#include "utils/timer.h"

namespace application_manager {

struct ServiceInfo {
  std::string service_id;
  uint32_t connection_key;
};

typedef std::shared_ptr<timer::Timer> TimerSPtr;

class AppServiceManager;

class RPCPassingHandler {
 public:
  RPCPassingHandler(AppServiceManager& asm_ref, ApplicationManager& am_ref);

  /**
   * @brief Class destructor
   */
  ~RPCPassingHandler();

  /**
   * @brief Check if function id is in the handled_rpcs list of an active app
   * service
   * @param function_id RPC function id
   * @return true if function id exists in handled_rpcs list of an active app
   * service
   */
  bool CanHandleFunctionID(int32_t function_id);

  /**
   * @brief Check if app services or core is being used to handle the RPC
   * @param correlation_id correlation id of RPC response
   * @return true if an app service was used to handle the RPC
   */
  bool IsPassThroughMessage(uint32_t correlation_id,
                            commands::Command::CommandSource source,
                            int32_t message_type);

  /**
   * @brief Function to handle sending and receiving RPC Passing
   * requests/responses
   * @param rpc_message RPC message SmartObject
   * @return true to stop current request/response and forward it
   */
  bool RPCPassThrough(smart_objects::SmartObject rpc_message);

 private:
  bool PerformNextRequest(uint32_t correlation_id);
  void OnPassThroughRequestTimeout();
  void AddRequestTimer(uint32_t correlation_id);
  void RemoveRequestTimer(uint32_t correlation_id);
  void ClearCompletedTimers();
  void ForwardRequestToMobile(uint32_t correlation_id);
  void ForwardRequestToCore(uint32_t correlation_id);
  void ForwardResponseToMobile(uint32_t correlation_id,
                               smart_objects::SmartObject response_message);
  void PopulateRPCRequestQueue(smart_objects::SmartObject request_message);

  AppServiceManager& app_service_manager_;
  ApplicationManager& app_manager_;
  sync_primitives::RecursiveLock rpc_request_queue_lock_;
  std::map<uint32_t,
           std::pair<smart_objects::SmartObject, std::deque<ServiceInfo> > >
      rpc_request_queue;
  sync_primitives::RecursiveLock timeout_queue_lock_;
  std::vector<std::pair<TimerSPtr, uint32_t> > timeout_queue_;
};

}  //  namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RPC_PASSING_HANDLER_H_
