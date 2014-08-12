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

CREATE_LOGGERPTR_GLOBAL(logger_, "RequestController")

RequestController::RequestController()
  : pool_state_(UNDEFINED)
  , pool_size_(profile::Profile::instance()->thread_pool_size())
  , watchdog_(NULL) {
  LOG4CXX_INFO(logger_, "RequestController::RequestController()");
  watchdog_ = new request_watchdog::RequestWatchdog;
  watchdog_->AddListener(this);
  InitializeThreadpool();
}

RequestController::~RequestController() {
  LOG4CXX_INFO(logger_, "RequestController::~RequestController()");
  if (pool_state_ != TPoolState::STOPPED) {
    DestroyThreadpool();
  }

  pool_.clear();
  request_list_.clear();
  pending_request_list_.clear();

  if (watchdog_) {
    watchdog_->RemoveListener(this);
    delete watchdog_; 
    watchdog_ = NULL;
  }
}

void RequestController::InitializeThreadpool()
{
  LOG4CXX_INFO(logger_, "RequestController::InitializeThreadpool()");
  // TODO: Consider lazy loading threads instead of creating all at once
  pool_state_ = TPoolState::STARTED;
  for (uint32_t i = 0; i < pool_size_; i++) {
    char name [50];
    snprintf(name, sizeof(name)/sizeof(name[0]),
             "Request thread %d", i);
    pool_.push_back(ThreadSharedPtr(new Thread(name, new Worker(this))));
    pool_[i]->start();
    LOG4CXX_INFO(logger_, "RequestController::InitializeThreadpool() " << name);
  }
}

void RequestController::DestroyThreadpool() {
  LOG4CXX_INFO(logger_, "RequestController::DestroyThreadpool()");
  request_list_lock_.Acquire();
  pool_state_ = TPoolState::STOPPED;
  request_list_lock_.Release();

  LOG4CXX_INFO(logger_, "Broadcasting STOP signal to all threads...");
  cond_var_.Broadcast(); // notify all threads we are shutting down
  for (uint32_t i = 0; i < pool_size_; i++) {
    pool_[i]->stop();
  }

  LOG4CXX_INFO(logger_, "Threads exited from the thread pool " << pool_size_);
}

RequestController::TResult RequestController::addRequest(
    const Request& request, const mobile_apis::HMILevel::eType& hmi_level) {
  LOG4CXX_INFO(logger_, "RequestController::addRequest()");

  RequestController::TResult result = RequestController::SUCCESS;
  {
    commands::CommandRequestImpl* request_impl =
      static_cast<commands::CommandRequestImpl*>(request.get());

    // TODO: discuss this solution with dklimenko
    // Call Init() first to be sure that all command-specific parameters
    // are up to date.
    if (request_impl->Init()) {

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

      const mobile_apis::HMILevel::eType hmi_level_none =
          mobile_apis::HMILevel::HMI_NONE;

      if (false == watchdog_->checkHMILevelTimeScaleMaxRequest(hmi_level_none,
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

        {
          AutoLock auto_lock(request_list_lock_);


          if (0 == request_impl->default_timeout()) {
            LOG4CXX_INFO(logger_, "Default timeout was set to 0."
                                  "Watchdog will not track this request.");
          } else {
            watchdog_->addRequest(new request_watchdog::RequestInfo(
                                    request_impl->function_id(),
                                    request_impl->connection_key(),
                                    request_impl->correlation_id(),
                                    request_impl->default_timeout(),
                                    hmi_level));

            LOG4CXX_INFO(logger_, "Adding request to watchdog. Default timeout is "
                         << request_impl->default_timeout());
          }

          request_list_.push_back(request);
          LOG4CXX_INFO(logger_, "RequestController size is "
                       << request_list_.size()
                       << " Pending request size is "
                       << pending_request_list_.size()
                     );
        }

        // wake up one thread that is waiting for a task to be available
        cond_var_.NotifyOne();
      }
    }
    else {
      LOG4CXX_ERROR(logger_, "The request didn't initilize.");
      result = RequestController::INIT_FAILED;
    }
  }
  return result;
}

void RequestController::terminateRequest(
    const uint32_t& mobile_correlation_id) {
  LOG4CXX_INFO(logger_, "RequestController::terminateRequest()");

  {
    AutoLock auto_lock(pending_request_list_lock_);
    std::list<Request>::iterator it = pending_request_list_.begin();
    for (; pending_request_list_.end() != it; ++it) {
      const commands::CommandRequestImpl* request_impl =
        static_cast<commands::CommandRequestImpl*>(it->get());
      if (request_impl->correlation_id() == mobile_correlation_id) {
        watchdog_->removeRequest(
          request_impl->connection_key(), request_impl->correlation_id());
        pending_request_list_.erase(it);
        break;
      }
    }
  }
}

void RequestController::terminateAppRequests(
    const uint32_t& app_id) {
  LOG4CXX_INFO(logger_, "RequestController::terminateAppRequests()");

  {
    AutoLock auto_lock(pending_request_list_lock_);
    std::list<Request>::iterator it = pending_request_list_.begin();
    while (pending_request_list_.end() != it) {
      commands::CommandRequestImpl* request_impl =
          static_cast<commands::CommandRequestImpl*>(it->get());
      if (request_impl->connection_key() == app_id) {
        request_impl->CleanUp();
        watchdog_->removeRequest(
          request_impl->connection_key(), request_impl->correlation_id());
        it = pending_request_list_.erase(it);
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
  watchdog_->updateRequestTimeout(connection_key,
                                  mobile_correlation_id,
                                  new_timeout);
}

void RequestController::onTimeoutExpired(
    const request_watchdog::RequestInfo& info) {
  LOG4CXX_INFO(logger_, "RequestController::onTimeoutExpired()");

  commands::CommandRequestImpl* request_impl = NULL;
  {
    AutoLock auto_lock(pending_request_list_lock_);
    std::list<Request>::iterator it = pending_request_list_.begin();
    for (; pending_request_list_.end() != it; ++it) {
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

RequestController::Worker::Worker(RequestController* requestController)
  : request_controller_(requestController)
  , stop_flag_(false)
  , is_active_(true) {
}

RequestController::Worker::~Worker() {
}

void RequestController::Worker::threadMain() {
  LOG4CXX_INFO(logger_, "RequestController::Worker::threadMain");
  while (!stop_flag_) {
    // Try to pick a request
    sync_primitives::AutoLock auto_lock(request_controller_->request_list_lock_);

    while ((request_controller_->pool_state_ != TPoolState::STOPPED) &&
           (request_controller_->request_list_.empty())) {
      // Wait until there is a task in the queue
      // Unlock mutex while wait, then lock it back when signaled
      LOG4CXX_INFO(logger_, "Unlocking and waiting");
      request_controller_->cond_var_.Wait(auto_lock);
      LOG4CXX_INFO(logger_, "Signaled and locking");
    }

    // If the thread was shutdown, return from here
    if (request_controller_->pool_state_ == TPoolState::STOPPED) {
      break;
    }

    Request request(request_controller_->request_list_.front());
    commands::CommandRequestImpl* request_impl = NULL;
    request_impl = static_cast<commands::CommandRequestImpl*>(request.get());
    request_controller_->request_list_.pop_front();

    //pending_request_list_.Acquire();
    request_controller_->pending_request_list_lock_.Acquire();
    request_controller_->pending_request_list_.push_back(request);
    request_controller_->pending_request_list_lock_.Release();

    AutoUnlock unlock(auto_lock);

    // execute
    if (request_impl->CheckPermissions()) {
      request_impl->Run();
    }
  }

  // for correct thread termination
  is_active_ = false;
}

bool RequestController::Worker::exitThreadMain() {
  stop_flag_ = true;
  return true;
}

}  //  namespace request_controller

}  //  namespace application_manager
