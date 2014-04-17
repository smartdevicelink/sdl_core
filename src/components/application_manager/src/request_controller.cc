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

#include "utils/logger.h"
#include "config_profile/profile.h"
#include "application_manager/request_controller.h"
#include "application_manager/commands/command_request_impl.h"

namespace application_manager {

namespace request_controller {
using namespace sync_primitives;

GETLOGGER(logger_, "RequestController")

RequestController::RequestController()
  : watchdog_(NULL) {
  LOG4CXX_INFO(logger_, "RequestController::RequestController()");
  watchdog_ = new request_watchdog::RequestWatchdog;
  watchdog_->AddListener(this);
}

RequestController::~RequestController() {
  LOG4CXX_INFO(logger_, "RequestController::~RequestController()");
  request_list_.clear();

  if (watchdog_) {
    watchdog_->RemoveListener(this);
    delete watchdog_; 
    watchdog_ = NULL;
  }
}

RequestController::TResult RequestController::addRequest(
    const Request& request, const mobile_apis::HMILevel::eType& hmi_level) {
  LOG4CXX_INFO(logger_, "RequestController::addRequest()");

  RequestController::TResult result = RequestController::SUCCESS;
  {
    AutoLock auto_lock(request_list_lock_);

    const commands::CommandRequestImpl* request_impl =
      static_cast<commands::CommandRequestImpl*>(request.get());

    const uint32_t& app_hmi_level_none_time_scale =
        profile::Profile::instance()->app_hmi_level_none_time_scale();

    const uint32_t& app_hmi_level_none_max_request_per_time_scale =
     profile::Profile::instance()->app_hmi_level_none_time_scale_max_requests();

    const uint32_t& app_time_scale =
        profile::Profile::instance()->app_time_scale();

    const uint32_t& max_request_per_time_scale =
        profile::Profile::instance()->app_time_scale_max_requests();

    const uint32_t& pending_requests_amount =
        profile::Profile::instance()->pending_requests_amount();

    const mobile_apis::HMILevel::eType hmi_level =
        mobile_apis::HMILevel::HMI_NONE;

    if (false == watchdog_->checkHMILevelTimeScaleMaxRequest(hmi_level,
                  request_impl->connection_key(), app_hmi_level_none_time_scale,
                  app_hmi_level_none_max_request_per_time_scale)) {
      LOG4CXX_ERROR(logger_, "Too many application requests in hmi level NONE");
      result = RequestController::NONE_HMI_LEVEL_MANY_REQUESTS;
    } else if (false == watchdog_->checkTimeScaleMaxRequest(
                                  request_impl->connection_key(),
                                  app_time_scale, max_request_per_time_scale)) {
      LOG4CXX_ERROR(logger_, "Too many application requests");
      result = RequestController::TOO_MANY_REQUESTS;
    } else if (pending_requests_amount == request_list_.size()) {
      LOG4CXX_ERROR(logger_, "Too many pending request");
      result = RequestController::TOO_MANY_PENDING_REQUESTS;
    } else {

      request_list_.push_back(request);

      LOG4CXX_INFO(logger_, "Adding request to watchdog. Default timeout is "
                   << request_impl->default_timeout());

      watchdog_->addRequest(new request_watchdog::RequestInfo(
                              request_impl->function_id(),
                              request_impl->connection_key(),
                              request_impl->correlation_id(),
                              request_impl->default_timeout(),
                              hmi_level));

      LOG4CXX_INFO(logger_, "Added request to watchdog.");
      LOG4CXX_INFO(logger_, "RequestController size is " << request_list_.size());
    }
  }

  return result;
}

void RequestController::terminateRequest(
    const uint32_t& mobile_correlation_id) {
  LOG4CXX_INFO(logger_, "RequestController::terminateRequest()");

  {
    AutoLock auto_lock(request_list_lock_);
    std::list<Request>::iterator it = request_list_.begin();
    for (; request_list_.end() != it; ++it) {
      const commands::CommandRequestImpl* request_impl =
        static_cast<commands::CommandRequestImpl*>(it->get());
      if (request_impl->correlation_id() == mobile_correlation_id) {
        watchdog_->removeRequest(
          request_impl->connection_key(), request_impl->correlation_id());
        request_list_.erase(it);
        break;
      }
    }
  }
}

void RequestController::terminateAppRequests(
    const uint32_t& app_id) {
  LOG4CXX_INFO(logger_, "RequestController::terminateAppRequests()");

  {
    AutoLock auto_lock(request_list_lock_);
    std::list<Request>::iterator it = request_list_.begin();
    while (request_list_.end() != it) {
      const commands::CommandRequestImpl* request_impl =
          static_cast<commands::CommandRequestImpl*>(it->get());
      if (request_impl->connection_key() == app_id) {
        watchdog_->removeRequest(
          request_impl->connection_key(), request_impl->correlation_id());
        it = request_list_.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void RequestController::updateRequestTimeout(
    const uint32_t& connection_key,
    const uint32_t& mobile_correlation_id,
    const uint32_t& new_timeout) {
  LOG4CXX_INFO(logger_, "RequestController::updateRequestTimeout()");

  watchdog_->updateRequestTimeout(connection_key,
                                  mobile_correlation_id,
                                  new_timeout);
}

void RequestController::onTimeoutExpired(
    const request_watchdog::RequestInfo& info) {
  LOG4CXX_INFO(logger_, "RequestController::onTimeoutExpired()");

  commands::CommandRequestImpl* request_impl = NULL;
  {
    AutoLock auto_lock(request_list_lock_);
    std::list<Request>::iterator it = request_list_.begin();
    for (; request_list_.end() != it; ++it) {
      request_impl = static_cast<commands::CommandRequestImpl*>(it->get());
      if (request_impl->correlation_id() == info.correlationID_ &&
          request_impl->connection_key() == info.connectionID_) {
        LOG4CXX_INFO(logger_, "Timeout for request id " << info.correlationID_
                     << " of application " << info.connectionID_ << " expired");
        break;
      }
    }
  }

  if (request_impl) {
    request_impl->onTimeOut();
  }
}

}  //  namespace request_controller

}  //  namespace application_manager
