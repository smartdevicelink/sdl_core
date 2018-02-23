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

#include "application_manager/system_time/system_time_handler_impl.h"

#include <algorithm>

#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"
#include "utils/logger.h"

namespace application_manager {

SystemTimeHandlerImpl::SystemTimeHandlerImpl(
    ApplicationManager& application_manager)
    : event_engine::EventObserver(application_manager.event_dispatcher())
    , is_utc_ready_(false)
    , schedule_request_(false)
    , system_time_listener_(NULL)
    , app_manager_(application_manager) {
  LOG4CXX_AUTO_TRACE(logger_);
  subscribe_on_event(
      hmi_apis::FunctionID::BasicCommunication_OnSystemTimeReady);
}

SystemTimeHandlerImpl::~SystemTimeHandlerImpl() {
  LOG4CXX_AUTO_TRACE(logger_);
  unsubscribe_from_all_events();
}

void SystemTimeHandlerImpl::DoSystemTimeQuery() {
  LOG4CXX_AUTO_TRACE(logger_);
  //  // Remove until next comment line
  //  last_time_ = time(NULL);
  //  if (system_time_listener_) {
  //    system_time_listener_->OnSystemTimeArrived(last_time_);
  //  }
  //  return;
  //  // =========================================================
  using namespace application_manager;

  sync_primitives::AutoLock lock(state_lock_);
  if (!is_utc_ready_) {
    LOG4CXX_INFO(logger_,
                 "Navi module is not yet ready."
                     << "Will process request once it became ready.");
    schedule_request_ = true;
    return;
  }
  SendTimeRequest();
}

void SystemTimeHandlerImpl::DoSubscribe(utils::SystemTimeListener* listener) {
  sync_primitives::AutoLock lock(system_time_listener_lock_);
  system_time_listener_ = listener;
}

void SystemTimeHandlerImpl::DoUnsubscribe(utils::SystemTimeListener* listener) {
  sync_primitives::AutoLock lock(system_time_listener_lock_);
  system_time_listener_ = NULL;
}

time_t SystemTimeHandlerImpl::FetchSystemTime() {
  LOG4CXX_AUTO_TRACE(logger_);
  return last_time_;
}

void SystemTimeHandlerImpl::SendTimeRequest() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  uint32_t correlation_id = app_manager_.GetNextHMICorrelationID();
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_GetSystemTime,
                     correlation_id);
  MessageHelper::SendGetSystemTimeRequest(correlation_id, app_manager_);
}

void SystemTimeHandlerImpl::on_event(
    const application_manager::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  using namespace hmi_apis::FunctionID;
  switch (event.id()) {
    case BasicCommunication_OnSystemTimeReady:
      OnSystemTimeReady();
      break;
    case BasicCommunication_GetSystemTime:
      OnSystemTimeResponse(event);
      break;
    default:
      break;
  }
}

void SystemTimeHandlerImpl::OnSystemTimeReady() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(state_lock_);
  is_utc_ready_ = true;
  if (schedule_request_) {
    SendTimeRequest();
    schedule_request_ = false;
  }
  unsubscribe_from_event(
      hmi_apis::FunctionID::BasicCommunication_OnSystemTimeReady);
}

void SystemTimeHandlerImpl::OnSystemTimeResponse(
    const application_manager::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();
  const smart_objects::SmartObject& system_time_so =
      message[strings::msg_params][hmi_response::system_time];

  struct tm system_time;
  memset(&system_time, 0, sizeof(struct tm));

  system_time.tm_sec = system_time_so[time_keys::second].asInt();
  system_time.tm_min = system_time_so[time_keys::minute].asInt();
  system_time.tm_hour = system_time_so[time_keys::hour].asInt();
  system_time.tm_mday = system_time_so[time_keys::day].asInt();
  system_time.tm_mon = system_time_so[time_keys::month].asInt() - 1;
  system_time.tm_year = system_time_so[time_keys::year].asInt() - 1900;

  last_time_ = mktime(&system_time);

  sync_primitives::AutoLock lock(system_time_listener_lock_);
  if (system_time_listener_) {
    system_time_listener_->OnSystemTimeArrived(last_time_);
  }
}

}  // namespace application_manager
