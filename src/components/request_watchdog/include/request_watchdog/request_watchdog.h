/**
* \file request_watchdog.h
* \brief RequestWatchdog class header file.
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

#ifndef SRC_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_WATCHDOG_H_
#define SRC_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_WATCHDOG_H_

#include <list>
#include <map>
#include "request_watchdog/watchdog.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/date_time.h"
#include "utils/synchronisation_primitives.h"

namespace request_watchdog {

class RequestWatchdog : public Watchdog {
 public:
  static Watchdog* getRequestWatchdog();

  virtual void addListener(WatchdogSubscriber* subscriber);
  virtual void removeListener(WatchdogSubscriber* subscriber);
  virtual void removeAllListeners();

  virtual void addRequest(RequestInfo requestInfo);
  virtual void removeRequest(RequestInfo requestInfo);
  virtual void removeAllRequests();

  virtual int getRegesteredRequestsNumber();

  ~RequestWatchdog();

 private:
  RequestWatchdog();

  static const int DEFAULT_CYCLE_TIMEOUT = 250000;
  static log4cxx::LoggerPtr logger_;

  static RequestWatchdog* sInstance_;
  static sync_primitives::SynchronisationPrimitives instanceMutex_;

  static void notifySubscribers(RequestInfo requestInfo);

  void startDispatcherThreadIfNeeded();
  void stopDispatcherThreadIfNeeded();

  static std::list<WatchdogSubscriber*> subscribers_;
  static sync_primitives::SynchronisationPrimitives subscribersListMutex_;

  static std::map<RequestInfo, struct timeval> requests_;
  static sync_primitives::SynchronisationPrimitives requestsMapMutex_;

  class QueueDispatcherThreadDelegate : public threads::ThreadDelegate {
   public:
     QueueDispatcherThreadDelegate();

     void threadMain();

   private:
     DISALLOW_COPY_AND_ASSIGN(QueueDispatcherThreadDelegate);
  };

  threads::Thread queueDispatcherThread;

  DISALLOW_COPY_AND_ASSIGN(RequestWatchdog);
};

}  //  namespace request_watchdog

#endif  // SRC_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_WATCHDOG_H_
