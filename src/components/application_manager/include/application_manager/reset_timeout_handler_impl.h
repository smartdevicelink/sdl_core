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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESET_TIMEOUT_HANDLER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESET_TIMEOUT_HANDLER_IMPL_H_

#include <map>
#include "application_manager/application_manager.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/reset_timeout_handler.h"

namespace application_manager {

namespace request_controller {

class ResetTimeoutHandlerImpl : public event_engine::EventObserver,
                                public ResetTimeoutHandler {
 public:
  ResetTimeoutHandlerImpl(ApplicationManager& application_manager);
  void AddRequest(uint32_t hmi_correlation_id,
                  uint32_t mobile_correlation_id,
                  uint32_t connection_key) OVERRIDE;
  void RemoveRequest(uint32_t hmi_correlation_id) OVERRIDE;
  void on_event(const event_engine::Event& event) OVERRIDE;

 private:
  std::map<uint32_t, std::pair<uint32_t, uint32_t> > requests_;
  ApplicationManager& application_manager_;
  mutable sync_primitives::Lock requests_lock_;
};

}  // namespace request_controller
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_TIMEOUT_HANDLER_IMPL_H_
