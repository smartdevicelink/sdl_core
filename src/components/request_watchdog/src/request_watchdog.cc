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
#include "request_watchdog/request_watchdog.h"

namespace request_watchdog {

const int RequestWatchdog::DEFAULT_CYCLE_TIMEOUT;

log4cxx::LoggerPtr RequestWatchdog::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger("RequestWatchdog"));

RequestWatchdog* RequestWatchdog::sInstance_ = 0;

std::list<WatchdogSubscriber*> RequestWatchdog::subscribers_;
std::map<RequestInfo, struct timeval> RequestWatchdog::requests_;
threads::SynchronisationPrimitives RequestWatchdog::instanceMutex_;
threads::SynchronisationPrimitives RequestWatchdog::subscribersListMutex_;
threads::SynchronisationPrimitives RequestWatchdog::requestsMapMutex_;

Watchdog* RequestWatchdog::getRequestWatchdog() {
  instanceMutex_.lock();

  if ( 0 == sInstance_ ) {
    sInstance_ = new RequestWatchdog;
  }

  instanceMutex_.unlock();

  return sInstance_;
}

RequestWatchdog::RequestWatchdog()
  : queueDispatcherThread("RequestQueueThread",
                          new QueueDispatcherThreadDelegate()) {
}

RequestWatchdog::~RequestWatchdog() {
  queueDispatcherThread.join();
}

void RequestWatchdog::addListener(WatchdogSubscriber* subscriber) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();

  subscribers_.push_back(subscriber);
  startDispatcherThreadIfNeeded();

  LOG4CXX_INFO(logger_, "Subscriber " << subscriber << " was added.");

  subscribersListMutex_.unlock();
}

void RequestWatchdog::removeListener(WatchdogSubscriber* subscriber) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();

  subscribers_.remove(subscriber);
  stopDispatcherThreadIfNeeded();

  LOG4CXX_INFO(logger_, "Subscriber " << subscriber << " was removed.");

  subscribersListMutex_.unlock();
}

void RequestWatchdog::removeAllListeners() {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();

  subscribers_.clear();

  subscribersListMutex_.unlock();

  queueDispatcherThread.stop();
}

void RequestWatchdog::notifySubscribers(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.lock();

  std::list<WatchdogSubscriber*>::iterator i = subscribers_.begin();

  while ( i != subscribers_.end() ) {
    (*i)->onTimeoutExpired(requestInfo);
    i++;
  }

  subscribersListMutex_.unlock();
}

void RequestWatchdog::addRequest(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();

  requests_.insert(std::pair<RequestInfo, struct timeval>(requestInfo,
                   date_time::DateTime::getCurrentTime()));

  LOG4CXX_INFO(logger_, "Add request "
                     << "\n ConnectionID : " << requestInfo.connectionID_
                     << "\n CorrelationID : " << requestInfo.correlationID_
                     << "\n FunctionID : " << requestInfo.functionID_
                     << "\n CustomTimeOut : " << requestInfo.customTimeout_
                     << "\n");

  startDispatcherThreadIfNeeded();

  requestsMapMutex_.unlock();
}

void RequestWatchdog::removeRequest(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();

  requests_.erase(requestInfo);

  LOG4CXX_INFO(logger_, "Delete request "
                     << "\n ConnectionID : " << requestInfo.connectionID_
                     << "\n CorrelationID : " << requestInfo.correlationID_
                     << "\n FunctionID : " << requestInfo.functionID_
                     << "\n CustomTimeOut : " << requestInfo.customTimeout_
                     << "\n");

  stopDispatcherThreadIfNeeded();

  requestsMapMutex_.unlock();
}

void RequestWatchdog::removeAllRequests() {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();

  requests_.clear();

  requestsMapMutex_.unlock();

  queueDispatcherThread.stop();
}

int RequestWatchdog::getRegesteredRequestsNumber() {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.lock();

  int ret = requests_.size();

  requestsMapMutex_.unlock();

  return ret;
}

void RequestWatchdog::startDispatcherThreadIfNeeded() {
  LOG4CXX_TRACE_ENTER(logger_);

  if ( !requests_.empty() && !subscribers_.empty() ) {
    queueDispatcherThread.start();
  }
}

void RequestWatchdog::stopDispatcherThreadIfNeeded() {
  LOG4CXX_TRACE_ENTER(logger_);

  if ( requests_.empty() || subscribers_.empty() ) {
    queueDispatcherThread.stop();
  }
}

RequestWatchdog::QueueDispatcherThreadDelegate::QueueDispatcherThreadDelegate()
  : threads::ThreadDelegate()
{}

void RequestWatchdog::QueueDispatcherThreadDelegate::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);
  std::vector<RequestInfo> expiredRequests = std::vector<RequestInfo>();
  std::map<RequestInfo, struct timeval>::iterator it;

  int cycleSleepInterval = DEFAULT_CYCLE_TIMEOUT;
  int cycleDuration;
  struct timeval cycleStartTime;

  while ( true ) {
    usleep(cycleSleepInterval);

    cycleStartTime = date_time::DateTime::getCurrentTime();

    requestsMapMutex_.lock();

    it = requests_.begin();

    while ( it != requests_.end() ) {
      LOG4CXX_INFO(logger_, "Checking timeout for the following request :"
                         << "\n ConnectionID : " << (*it).first.connectionID_
                         << "\n CorrelationID : " << (*it).first.correlationID_
                         << "\n FunctionID : " << (*it).first.functionID_
                         << "\n CustomTimeOut : " << (*it).first.customTimeout_
                         << "\n");

      if ( (*it).first.customTimeout_ <
          date_time::DateTime::calculateTimeSpan((*it).second) ) {
          // Request is expired - notify all subscribers and remove request

        LOG4CXX_INFO(logger_, "Timeout had expired for the following request :"
                         << "\n ConnectionID : " << (*it).first.connectionID_
                         << "\n CorrelationID : " << (*it).first.correlationID_
                         << "\n FunctionID : " << (*it).first.functionID_
                         << "\n CustomTimeOut : " << (*it).first.customTimeout_
                         << "\n");

        requestsMapMutex_.unlock();
        notifySubscribers((*it).first);
        expiredRequests.push_back((*it).first);
        requestsMapMutex_.lock();
      }
      it++;
    }

    requestsMapMutex_.unlock();

    for_each(expiredRequests.begin(), expiredRequests.end(),
             std::bind1st(std::mem_fun(&RequestWatchdog::removeRequest),
             RequestWatchdog::getRequestWatchdog()));

    expiredRequests.clear();

    cycleDuration = date_time::DateTime::calculateTimeSpan(cycleStartTime);
    cycleSleepInterval += DEFAULT_CYCLE_TIMEOUT *
          (cycleDuration / DEFAULT_CYCLE_TIMEOUT + 1);
  }
}

}  //  namespace request_watchdog
