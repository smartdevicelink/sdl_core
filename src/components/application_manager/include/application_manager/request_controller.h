/**
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_H_

#include <list>
#include "utils/synchronisation_primitives.h"
#include "request_watchdog/request_watchdog.h"
#include "application_manager/commands/command.h"
#include "request_watchdog/watchdog_subscriber.h"

namespace application_manager {

namespace request_controller {

/*
 * @brief Typedef for active mobile request
 *
 */
typedef utils::SharedPtr<commands::Command> Request;

/*
 * @brief RequestController class is used to control currently active mobile
 * requests.
 */
class RequestController: public request_watchdog::WatchdogSubscriber  {
 public:

  /*
   * @brief Class constructor
   *
   */
  RequestController();

  /*
   * @brief Class destructor
   *
   */
  virtual ~RequestController();

  /*
   * @brief Adds request to queue
   *
   * @param request Active mobile request
   *
   */
  void addRequest(const Request& request);

  /*
   * @brief Removes request from queue
   *
   * @param mobile_corellation_id Active mobile request correlation ID
   *
   */
  void terminateRequest(unsigned int mobile_correlation_id);

  /**
   * @ Updates request timeout
   *
   * @param connection_key Connection key of application
   * @param mobile_correlation_id Correlation ID of the mobile request
   * @param new_timeout_value New timeout to be set
   */
  void updateRequestTimeout(unsigned int connection_key,
                            unsigned int mobile_correlation_id,
                            unsigned int new_timeout);

  /*
   * @brief Notify subscriber that expired entry should be removed
   * using Watchdog::removeRequest(int connection_key, int correlation_id)
   *
   * @param RequestInfo Request related information
   */
  void onTimeoutExpired(request_watchdog::RequestInfo info);

 protected:

 private:

  std::list<Request>                          request_list_;
  sync_primitives::SynchronisationPrimitives  list_mutex_;
  request_watchdog::Watchdog*                 watchdog_;

  DISALLOW_COPY_AND_ASSIGN(RequestController);
};

}  // namespace request_controller

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_CONTROLLER_H_
