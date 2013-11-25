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

#include <iterator>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include "request_watchdog/request_watchdog.h"
#include "config_profile/profile.h"

namespace request_watchdog {
using namespace sync_primitives;

const int RequestWatchdog::DEFAULT_CYCLE_TIMEOUT;

log4cxx::LoggerPtr RequestWatchdog::logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("RequestWatchdog"));

Watchdog* RequestWatchdog::instance() {
  static RequestWatchdog instance;
  return &instance;
}

RequestWatchdog::RequestWatchdog()
  : queueDispatcherThread("RequestQueueThread",
                          new QueueDispatcherThreadDelegate()) {
  LOG4CXX_TRACE_ENTER(logger_);
  queueDispatcherThread.start();
}

RequestWatchdog::~RequestWatchdog() {
  LOG4CXX_INFO(logger_, "RequestWatchdog destructor.");
  stopDispatcherThreadIfNeeded();
  queueDispatcherThread.join();
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

void RequestWatchdog::removeRequest(int connection_key,
                                    int correlation_id) {
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

void RequestWatchdog::updateRequestTimeout(int connection_key,
                                           int correlation_id,
                                           int new_timeout_value) {
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

bool RequestWatchdog::timeScaleMaxRequestExceed(int connection_key) {
  LOG4CXX_TRACE_ENTER(logger_);

  bool result = false;
  {
    AutoLock auto_lock(requestsLock_);
    TimevalStruct end = date_time::DateTime::getCurrentTime();
    TimevalStruct start;
    start.tv_sec = end.tv_sec - profile::Profile::instance()->app_time_scale();

    TimeScale scale(start, end, connection_key);
    int count = 0;

    count = count_if (requests_.begin(), requests_.end(), scale);
    int max_request_per_time_scale =
        profile::Profile::instance()->app_time_scale_max_requests();

    if (count == max_request_per_time_scale ) {
      LOG4CXX_ERROR(logger_, "Requests count " << count
                    << " exceed application limit" << max_request_per_time_scale);
      result = true;
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

int RequestWatchdog::getRegesteredRequestsNumber() {
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

RequestWatchdog::QueueDispatcherThreadDelegate::QueueDispatcherThreadDelegate()
  : threads::ThreadDelegate()
{}

void RequestWatchdog::QueueDispatcherThreadDelegate::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);
  std::map<RequestInfo*, TimevalStruct>::iterator it;
  std::map<RequestInfo*, TimevalStruct>::iterator it_temp;

  int cycleSleepInterval = DEFAULT_CYCLE_TIMEOUT;
  int cycleDuration;
  TimevalStruct cycleStartTime;

  RequestWatchdog* instance = static_cast<RequestWatchdog*>(
                                RequestWatchdog::instance());
  if (!instance) {
    LOG4CXX_INFO(logger_, "Cannot get instance of RequestWatchdog.");
    return;
  }

  while (true) {
    usleep(cycleSleepInterval);

    cycleStartTime = date_time::DateTime::getCurrentTime();

    {
      AutoLock auto_lock(instance->requestsLock_);

      it = instance->requests_.begin();

      while (it != instance->requests_.end()) {
        if (it->first->delayed_delete_) {
          if (instance->requests_.begin() == it) {
            delete it->first;
            instance->requests_.erase(it);
            it = instance->requests_.begin();
          } else {
            it_temp = --it;
            delete(++it)->first;
            instance->requests_.erase(it);
            it = ++it_temp;
          }
          continue;
        }
        LOG4CXX_INFO(logger_, "Checking timeout for the following request :"
                     << "\n ConnectionID : " << (*it).first->connectionID_
                     << "\n CorrelationID : " << (*it).first->correlationID_
                     << "\n FunctionID : " << (*it).first->functionID_
                     << "\n CustomTimeOut : " << (*it).first->customTimeout_
                     << "\n");

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
            instance->notifySubscribers(*(it->first));
          }

        }
        it++;
      }
    }

    cycleDuration = date_time::DateTime::calculateTimeSpan(cycleStartTime);
    cycleSleepInterval += DEFAULT_CYCLE_TIMEOUT *
                          (cycleDuration / DEFAULT_CYCLE_TIMEOUT + 1);
  }
}

}  //  namespace request_watchdog
