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
#include "system.h"
#include "request_watchdog/request_watchdog.h"

using namespace std;
using namespace date_time;
using namespace System;

namespace request_watchdog {

const int RequestWatchdog::DEFAULT_CYCLE_TIMEOUT;

log4cxx::LoggerPtr RequestWatchdog::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger( "RequestWatchdog"));

RequestWatchdog* RequestWatchdog::sInstance_ = 0;

Watchdog* RequestWatchdog::getRequestWatchdog() {

  instanceMutex_.Lock();

  if(0 == sInstance_) {
    sInstance_ = new RequestWatchdog;
  }

  instanceMutex_.Unlock();

  return sInstance_;
}

RequestWatchdog::RequestWatchdog()
 : queueDispatcherThread("RequestQueueThread", new QueueDispatcherThreadDelegate()) {
}

RequestWatchdog::~RequestWatchdog() {
  queueDispatcherThread.join();
}

void RequestWatchdog::addListener(WatchdogSubscriber* subscriber) {

  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.Lock();

  subscribers_.push_back(subscriber);
  startDispatcherThreadIfNeeded();

  subscribersListMutex_.Unlock();
}

void RequestWatchdog::removeListener(WatchdogSubscriber* subscriber) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.Lock();

  subscribers_.remove(subscriber);
  stopDispatcherThreadIfNeeded();

  subscribersListMutex_.Unlock();
}

void RequestWatchdog::removeAllListeners() {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.Lock();

  subscribers_.clear();

  subscribersListMutex_.Unlock();

  queueDispatcherThread.stop();
}

void RequestWatchdog::notifySubscribers(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  subscribersListMutex_.Lock();

  list<WatchdogSubscriber*>::iterator i = subscribers_.begin();

  while(i != subscribers_.end()) {
    (*i)->onTimeoutExpired(requestInfo);
    i++;
  }

  subscribersListMutex_.Unlock();
}

void RequestWatchdog::addRequest(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.Lock();

  requests_.insert(pair<RequestInfo, struct timeval>(requestInfo,
                   DateTime::getCurrentTime()));

  startDispatcherThreadIfNeeded();

  requestsMapMutex_.Unlock();
}

void RequestWatchdog::removeRequest(RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.Lock();

  requests_.erase(requestInfo);

  stopDispatcherThreadIfNeeded();

  requestsMapMutex_.Unlock();
}

void RequestWatchdog::removeAllRequests() {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.Lock();

  requests_.clear();

  requestsMapMutex_.Unlock();

  queueDispatcherThread.stop();
}

int RequestWatchdog::getRegesteredRequestsNumber() {
  LOG4CXX_TRACE_ENTER(logger_);

  requestsMapMutex_.Lock();

  int ret = requests_.size();

  requestsMapMutex_.Unlock();

  return ret;
}

void RequestWatchdog::startDispatcherThreadIfNeeded() {
  LOG4CXX_TRACE_ENTER(logger_);

  if(!requests_.empty() && !subscribers_.empty()) {
    queueDispatcherThread.start();
  }
}

void RequestWatchdog::stopDispatcherThreadIfNeeded() {
  LOG4CXX_TRACE_ENTER(logger_);

  if(requests_.empty() || subscribers_.empty()) {
    queueDispatcherThread.stop();
  }
}

void RequestWatchdog::QueueDispatcherThreadDelegate::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);
  std::map<RequestInfo, struct timeval>::iterator it;

  int cycleSleepInterval = DEFAULT_CYCLE_TIMEOUT;
  int cycleDuration;
  struct timeval cycleStartTime;

  while(true) {

    usleep(cycleSleepInterval);

    cycleStartTime = DateTime::getCurrentTime();

    requestsMapMutex_.Lock();

    it = requests_.begin();

    while(it != requests_.end()) {

      if((*it).first.customTimeout_ <
         DateTime::calculateTimeSpan((*it).second)) {

      // Request is expired - notify all subscribers and remove request
      notifySubscribers((*it).first);
      RequestWatchdog::getRequestWatchdog()->removeRequest((*it).first);
    }

    requestsMapMutex_.Unlock();

    cycleDuration = DateTime::calculateTimeSpan(cycleStartTime);

    cycleSleepInterval += DEFAULT_CYCLE_TIMEOUT *
         (cycleDuration / DEFAULT_CYCLE_TIMEOUT + 1);

    }

  }
}

}
