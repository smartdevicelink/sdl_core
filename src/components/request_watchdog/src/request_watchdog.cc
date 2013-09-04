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

namespace request_watchdog {

const int RequestWatchdog::DEFAULT_CYCLE_TIMEOUT;

log4cxx::LoggerPtr RequestWatchdog::logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("RequestWatchdog"));

/*RequestWatchdog* RequestWatchdog::sInstance_ = 0;

std::list<WatchdogSubscriber*> RequestWatchdog::subscribers_;
std::map<RequestInfo, struct timeval> RequestWatchdog::requests_;
sync_primitives::SynchronisationPrimitives RequestWatchdog::instanceMutex_;
sync_primitives::SynchronisationPrimitives
RequestWatchdog::subscribersListMutex_;
sync_primitives::SynchronisationPrimitives RequestWatchdog::requestsMapMutex_;*/

Watchdog* RequestWatchdog::instance() {
  static RequestWatchdog instance;
  return &instance;
}

RequestWatchdog::RequestWatchdog()
  : queueDispatcherThread("RequestQueueThread",
                          new QueueDispatcherThreadDelegate()) {
  LOG4CXX_TRACE_ENTER(logger_);
  subscribersListMutex_.init();
  requestsMapMutex_.init();

  queueDispatcherThread.start();
}

RequestWatchdog::~RequestWatchdog() {
  stopDispatcherThreadIfNeeded();
  queueDispatcherThread.join();
}

void RequestWatchdog::AddListener(WatchdogSubscriber* subscriber) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is locked");

  subscribers_.push_back(subscriber);
  //startDispatcherThreadIfNeeded();

  LOG4CXX_INFO(logger_, "Subscriber " << subscriber << " was added.");

  subscribersListMutex_.unlock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is unlocked");
}

void RequestWatchdog::RemoveListener(WatchdogSubscriber* subscriber) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is locked");

  subscribers_.remove(subscriber);
  //stopDispatcherThreadIfNeeded();

  LOG4CXX_INFO(logger_, "Subscriber " << subscriber << " was removed.");

  subscribersListMutex_.unlock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is unlocked");
}

void RequestWatchdog::removeAllListeners() {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is locked");

  subscribers_.clear();

  subscribersListMutex_.unlock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is unlocked");

  queueDispatcherThread.stop();
}

void RequestWatchdog::notifySubscribers(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is locked");

  std::list<WatchdogSubscriber*>::iterator i = subscribers_.begin();

  while (i != subscribers_.end()) {
    (*i)->onTimeoutExpired(requestInfo);
    i++;
  }

  subscribersListMutex_.unlock();
  LOG4CXX_INFO(logger_, "subscribersListMutex_  is unlocked");
}

void RequestWatchdog::addRequest(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is locked");

  requests_.insert(std::pair<RequestInfo, struct timeval>(requestInfo,
                   date_time::DateTime::getCurrentTime()));

  LOG4CXX_INFO(logger_, "Add request "
               << "\n ConnectionID : " << requestInfo.connectionID_
               << "\n CorrelationID : " << requestInfo.correlationID_
               << "\n FunctionID : " << requestInfo.functionID_
               << "\n CustomTimeOut : " << requestInfo.customTimeout_
               << "\n");

  //startDispatcherThreadIfNeeded();

  requestsMapMutex_.unlock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is unlocked");
}

void RequestWatchdog::removeRequest(int connection_key,
                                    int correlation_id) {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is locked");

  for (std::map<RequestInfo, struct timeval>::iterator it =
         requests_.begin();
       requests_.end() != it;
       ++it) {
    if (it->first.connectionID_ == connection_key
        && it->first.correlationID_ == correlation_id) {
      LOG4CXX_INFO(logger_, "Delete request "
                   << "\n ConnectionID : " << it->first.connectionID_
                   << "\n CorrelationID : " << it->first.correlationID_
                   << "\n FunctionID : " << it->first.functionID_
                   << "\n CustomTimeOut : " << it->first.customTimeout_
                   << "\n");
      requests_.erase(it);
      break;
    }
  }

  //stopDispatcherThreadIfNeeded();

  requestsMapMutex_.unlock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is unlocked");
}

void RequestWatchdog::removeAllRequests() {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is locked");

  requests_.clear();

  requestsMapMutex_.unlock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is unlocked");

  queueDispatcherThread.stop();
}

int RequestWatchdog::getRegesteredRequestsNumber() {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is locked");

  int ret = requests_.size();

  requestsMapMutex_.unlock();
  LOG4CXX_INFO(logger_, "requestsMapMutex_  is unlocked");

  return ret;
}

void RequestWatchdog::startDispatcherThreadIfNeeded() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (!requests_.empty() && !subscribers_.empty()) {
    queueDispatcherThread.start();
  }
}

void RequestWatchdog::stopDispatcherThreadIfNeeded() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (requests_.empty() || subscribers_.empty()) {
    queueDispatcherThread.stop();
  }
}

RequestWatchdog::QueueDispatcherThreadDelegate::QueueDispatcherThreadDelegate()
  : threads::ThreadDelegate()
{}

void RequestWatchdog::QueueDispatcherThreadDelegate::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);
  //std::vector<RequestInfo> expiredRequests = std::vector<RequestInfo>();
  std::map<RequestInfo, struct timeval>::iterator it;
  std::map<RequestInfo, struct timeval>::iterator it_temp;

  int cycleSleepInterval = DEFAULT_CYCLE_TIMEOUT;
  int cycleDuration;
  struct timeval cycleStartTime;

  RequestWatchdog* instance = static_cast<RequestWatchdog*>(
                                RequestWatchdog::instance());
  if (!instance) {
    LOG4CXX_INFO(logger_, "Cannot get instance of RequestWatchdog.");
    return;
  }

  while (true) {
    usleep(cycleSleepInterval);

    cycleStartTime = date_time::DateTime::getCurrentTime();

    instance->requestsMapMutex_.lock();
    LOG4CXX_INFO(logger_, "requestsMapMutex_  is locked");

    it = instance->requests_.begin();

    while (it != instance->requests_.end()) {
      LOG4CXX_INFO(logger_, "Checking timeout for the following request :"
                   << "\n ConnectionID : " << (*it).first.connectionID_
                   << "\n CorrelationID : " << (*it).first.correlationID_
                   << "\n FunctionID : " << (*it).first.functionID_
                   << "\n CustomTimeOut : " << (*it).first.customTimeout_
                   << "\n");

      if ((*it).first.customTimeout_ <
          date_time::DateTime::calculateTimeSpan((*it).second)) {
        // Request is expired - notify all subscribers and remove request

        LOG4CXX_INFO(logger_, "Timeout had expired for the following request :"
                     << "\n ConnectionID : " << (*it).first.connectionID_
                     << "\n CorrelationID : " << (*it).first.correlationID_
                     << "\n FunctionID : " << (*it).first.functionID_
                     << "\n CustomTimeOut : " << (*it).first.customTimeout_
                     << "\n");

        instance->requestsMapMutex_.unlock();
        instance->notifySubscribers(it->first);
        instance->requestsMapMutex_.lock();

      }
      it++;
    }

    instance->requestsMapMutex_.unlock();
    LOG4CXX_INFO(logger_, "requestsMapMutex_  is unlocked");

    /*for_each(expiredRequests.begin(), expiredRequests.end(),
             std::bind1st(std::mem_fun(&RequestWatchdog::removeRequest),
                          RequestWatchdog::instance()));

    expiredRequests.clear();*/

    cycleDuration = date_time::DateTime::calculateTimeSpan(cycleStartTime);
    cycleSleepInterval += DEFAULT_CYCLE_TIMEOUT *
                          (cycleDuration / DEFAULT_CYCLE_TIMEOUT + 1);
  }
}

}  //  namespace request_watchdog
