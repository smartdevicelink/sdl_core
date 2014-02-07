/**
* \file request_watchdog.cc
* \brief RequetWatchdog class source file.
*
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

#include <unistd.h>
#include <iterator>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include "request_watchdog/request_watchdog.h"

namespace request_watchdog {
using namespace sync_primitives;

const int32_t RequestWatchdog::DEFAULT_CYCLE_TIMEOUT;

log4cxx::LoggerPtr RequestWatchdog::logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("RequestWatchdog"));
/*
Watchdog* RequestWatchdog::instance() {
  static RequestWatchdog instnc;
  return &instnc;
}
*/
RequestWatchdog::RequestWatchdog()
  : queueDispatcherThread("RequestQueueThread",
                          new QueueDispatcherThreadDelegate(this)) {
  LOG4CXX_TRACE_ENTER(logger_);
  queueDispatcherThread.start();
}

RequestWatchdog::~RequestWatchdog() {
  //printf("\n\n\n after begin \n\n\n");
  LOG4CXX_INFO(logger_, "RequestWatchdog destructor.");
  stopDispatcherThreadIfNeeded();
  //printf("\n\n\n after join \n\n\n");
}

void RequestWatchdog::AddListener(WatchdogSubscriber* subscriber) {
  LOG4CXX_TRACE_ENTER(logger_);
  {
    AutoLock auto_lock(subscribersLock_);

    subscribers_.push_back(subscriber);

    LOG4CXX_INFO(logger_, "Subscriber " << subscriber << " was added.");
  }
}

void RequestWatchdog::RemoveListener(WatchdogSubscriber* subscriber) {
  LOG4CXX_TRACE_ENTER(logger_);

  {
    AutoLock auto_lock(subscribersLock_);

    subscribers_.remove(subscriber);

    LOG4CXX_INFO(logger_, "Subscriber " << subscriber << " was removed.");
  }
}

void RequestWatchdog::removeAllListeners() {
  LOG4CXX_TRACE_ENTER(logger_);

  {
    AutoLock auto_lock(subscribersLock_);
    subscribers_.clear();
  }

  queueDispatcherThread.stop();
}

void RequestWatchdog::notifySubscribers(const RequestInfo& requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  {
    AutoLock auto_lock(subscribersLock_);
    std::list<WatchdogSubscriber*>::iterator i = subscribers_.begin();

    while (i != subscribers_.end()) {
      (*i)->onTimeoutExpired(requestInfo);
      i++;
    }
  }
}

void RequestWatchdog::addRequest(RequestInfo* requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  {
    AutoLock auto_lock(requestsLock_);
    requests_.insert(std::pair<RequestInfo*, TimevalStruct>(requestInfo,
                     date_time::DateTime::getCurrentTime()));

    LOG4CXX_INFO(logger_, "Add request "
                 << "\n ConnectionID : " << requestInfo->connectionID_
                 << "\n CorrelationID : " << requestInfo->correlationID_
                 << "\n FunctionID : " << requestInfo->functionID_
                 << "\n CustomTimeOut : " << requestInfo->customTimeout_
                 << "\n");
  }
}

void RequestWatchdog::removeRequest(int32_t connection_key,
                                    int32_t correlation_id) {
  LOG4CXX_TRACE_ENTER(logger_);
  {
    AutoLock auto_lock(requestsLock_);
    for (std::map<RequestInfo*, TimevalStruct>::iterator it =
           requests_.begin();
         requests_.end() != it;
         ++it) {
      if (it->first->connectionID_ == connection_key
          && it->first->correlationID_ == correlation_id) {
        LOG4CXX_INFO(logger_, "Delete request "
                     << "\n ConnectionID : " << it->first->connectionID_
                     << "\n CorrelationID : " << it->first->correlationID_
                     << "\n FunctionID : " << it->first->functionID_
                     << "\n CustomTimeOut : " << it->first->customTimeout_
                     << "\n");
        if (!it->first->delayed_delete_) {
          delete it->first;
          requests_.erase(it);
        }
        break;
      }
    }
  }
}

void RequestWatchdog::updateRequestTimeout(int32_t connection_key,
                                           int32_t correlation_id,
                                           int32_t new_timeout_value) {
  LOG4CXX_TRACE_ENTER(logger_);

  {
    AutoLock auto_lock(requestsLock_);

    for (std::map<RequestInfo*, TimevalStruct>::iterator it =
           requests_.begin();
         requests_.end() != it;
         ++it) {
      if (it->first->connectionID_ == connection_key
          && it->first->correlationID_ == correlation_id) {
        LOG4CXX_INFO(logger_, "Update request's expiration timeout "
                     << "\n ConnectionID : " << it->first->connectionID_
                     << "\n CorrelationID : " << it->first->correlationID_
                     << "\n FunctionID : " << it->first->functionID_
                     << "\n CustomTimeOut : " << it->first->customTimeout_
                     << "\n");
        it->first->customTimeout_ = new_timeout_value;
        it->second = date_time::DateTime::getCurrentTime();
        it->first->delayed_delete_ = false;
        break;
      }
    }

  }
}

bool RequestWatchdog::checkTimeScaleMaxRequest(
                              const int32_t& connection_key,
                              const uint32_t& app_time_scale,
                              const uint32_t& max_request_per_time_scale) {
  LOG4CXX_TRACE_ENTER(logger_);

  bool result = true;
  {
    AutoLock auto_lock(requestsLock_);
    TimevalStruct end = date_time::DateTime::getCurrentTime();
    TimevalStruct start;
    start.tv_sec = end.tv_sec - app_time_scale;

    TimeScale scale(start, end, connection_key);
    int32_t count = 0;

    count = count_if (requests_.begin(), requests_.end(), scale);

    if (count == max_request_per_time_scale ) {
      LOG4CXX_ERROR(logger_, "Requests count " << count <<
                    " exceed application limit" << max_request_per_time_scale);
      result = false;
    }

  }

  return result;
}

bool RequestWatchdog::checkHMILevelTimeScaleMaxRequest(
                              const int32_t& hmi_level,
                              const int32_t& connection_key,
                              const uint32_t& app_time_scale,
                              const uint32_t& max_request_per_time_scale) {
  LOG4CXX_TRACE_ENTER(logger_);

  bool result = true;
  {
    AutoLock auto_lock(requestsLock_);
    TimevalStruct end = date_time::DateTime::getCurrentTime();
    TimevalStruct start;
    start.tv_sec = end.tv_sec - app_time_scale;

    HMILevelTimeScale scale(start, end, connection_key, hmi_level);
    int32_t count = 0;

    count = count_if (requests_.begin(), requests_.end(), scale);

    if (count == max_request_per_time_scale ) {
      LOG4CXX_ERROR(logger_, "Requests count " << count <<
                    " exceed application limit" << max_request_per_time_scale
                    << " in hmi level " << hmi_level);
      result = false;
    }

  }

  return result;
}

void RequestWatchdog::removeAllRequests() {
  LOG4CXX_TRACE_ENTER(logger_);

  {
  AutoLock auto_lock(requestsLock_);
    for (std::map<RequestInfo*, TimevalStruct>::iterator it = requests_.begin();
         requests_.end() != it; ++it) {
      delete it->first;
    }
    requests_.clear();
  }
  queueDispatcherThread.stop();
}

int32_t RequestWatchdog::getRegesteredRequestsNumber() {
  LOG4CXX_TRACE_ENTER(logger_);
  {
    AutoLock auto_lock(requestsLock_);
    return requests_.size();
  }
}

void RequestWatchdog::startDispatcherThreadIfNeeded() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (!requests_.empty() && !subscribers_.empty()) {
    queueDispatcherThread.start();
  }
}

void RequestWatchdog::stopDispatcherThreadIfNeeded() {
  LOG4CXX_INFO(logger_, "Stop Watchdog thread.");
  queueDispatcherThread.stop();
}

RequestWatchdog::QueueDispatcherThreadDelegate::QueueDispatcherThreadDelegate(RequestWatchdog* inRequestWatchdog)
  : threads::ThreadDelegate()
  , stop_flag_(false)
  ,requestWatchdog_(inRequestWatchdog)
{
}

void RequestWatchdog::QueueDispatcherThreadDelegate::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);
  std::map<RequestInfo*, TimevalStruct>::iterator it;
  std::map<RequestInfo*, TimevalStruct>::iterator it_temp;

  int32_t cycleSleepInterval = DEFAULT_CYCLE_TIMEOUT;

  //RequestWatchdog* instnc = static_cast<RequestWatchdog*>(
  //                              RequestWatchdog::instance());
  if (!requestWatchdog_) {
    LOG4CXX_INFO(logger_, "Cannot get instance of RequestWatchdog.");
    return;
  }

  while (!stop_flag_) {
    usleep(cycleSleepInterval);

    {
      AutoLock auto_lock(requestWatchdog_->requestsLock_);

      it = requestWatchdog_->requests_.begin();

      while (it != requestWatchdog_->requests_.end()) {
        if (it->first->delayed_delete_) {
          if (requestWatchdog_->requests_.begin() == it) {
            delete it->first;
            requestWatchdog_->requests_.erase(it);
            it = requestWatchdog_->requests_.begin();
          } else {
            it_temp = --it;
            delete(++it)->first;
            requestWatchdog_->requests_.erase(it);
            it = ++it_temp;
          }
          continue;
        }
/*        LOG4CXX_INFO(logger_, "Checking timeout for the following request :"
                     << "\n ConnectionID : " << (*it).first->connectionID_
                     << "\n CorrelationID : " << (*it).first->correlationID_
                     << "\n FunctionID : " << (*it).first->functionID_
                     << "\n CustomTimeOut : " << (*it).first->customTimeout_
                     << "\n");*/

        if ((*it).first->customTimeout_ <
            date_time::DateTime::calculateTimeSpan((*it).second)) {
          // Request is expired - notify all subscribers and remove request

          it->first->delayed_delete_ = true;

          LOG4CXX_INFO(logger_, "Timeout had expired for the following request :"
                       << "\n ConnectionID : " << (*it).first->connectionID_
                       << "\n CorrelationID : " << (*it).first->correlationID_
                       << "\n FunctionID : " << (*it).first->functionID_
                       << "\n CustomTimeOut : " << (*it).first->customTimeout_
                       << "\n");

          {
            AutoUnlock auto_unlock(auto_lock);
            requestWatchdog_->notifySubscribers(*(it->first));
          }

        }
        it++;
      }
    }

  }
}

bool RequestWatchdog::QueueDispatcherThreadDelegate::exitThreadMain() {
  stop_flag_ = true;
  return true;
}

}  //  namespace request_watchdog
