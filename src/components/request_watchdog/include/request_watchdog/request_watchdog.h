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
#include "utils/lock.h"

namespace request_watchdog {

class RequestWatchdog : public Watchdog {
  public:

    static Watchdog* instance();

    virtual void AddListener(WatchdogSubscriber* subscriber);
    virtual void RemoveListener(WatchdogSubscriber* subscriber);
    virtual void removeAllListeners();

    /*
     * @brief Adds request
     *
     * @brief requestInfo Request info (connection key, request correlation id,
     * request id, watchdog timeout for request, current application hmi level
     */
    virtual void addRequest(RequestInfo* requestInfo);

    /*
     * @brief Removes corresponding request
     *
     * @brief connection_key    Application connection key
     * @brief correlation_id    Mobile request correlation ID
     */
    virtual void removeRequest(int connection_key,
                               int correlation_id);

    /*
     * @brief Update request watchdog timeout
     *
     * @brief connection_key    Application connection key
     * @brief correlation_id    Mobile request correlation ID
     * @brief new_timeout_value New value of request watchdog timeout
     */
    virtual void updateRequestTimeout(int connection_key,
                                      int correlation_id,
                                      int new_timeout_value);

    /*
     * @brief Check if amount of requests during time scale for application
     * doesn't exceed limit.
     *
     * @brief connection_key Application ID
     * @brief app_time_scale Configured time scale for application
     * @brief max_request_per_time_scale Configured max request amount for
     * application time scale
     *
     * @return TRUE if amount of request doesn't exceed limit, otherwise FALSE
     */
    virtual bool checkTimeScaleMaxRequest(
                                const int& connection_key,
                                const unsigned int& app_time_scale,
                                const unsigned int& max_request_per_time_scale);

    /*
     * @brief Check if amount of requests during time scale for application in
     * specified hmi level doesn't exceed limit.
     *
     * @brief hmi_level      Application hmi level(NONE)
     * @brief connection_key Application ID
     * @brief app_time_scale Configured time scale for application
     * @brief max_request_per_time_scale Configured max request amount for
     * application time scale
     *
     * @return TRUE if amount of request doesn't exceed limit, otherwise FALSE
     */
    virtual bool checkHMILevelTimeScaleMaxRequest(
                                const int& hmi_level,
                                const int& connection_key,
                                const unsigned int& app_time_scale,
                                const unsigned int& max_request_per_time_scale);

    /*
     * @brief Removes all requests
     */
    virtual void removeAllRequests();

    virtual int getRegesteredRequestsNumber();

    ~RequestWatchdog();

  private:

    RequestWatchdog();

    void notifySubscribers(const RequestInfo& requestInfo);

    void startDispatcherThreadIfNeeded();

    void stopDispatcherThreadIfNeeded();

    friend class QueueDispatcherThreadDelegate;

    class QueueDispatcherThreadDelegate : public threads::ThreadDelegate {
      public:
        QueueDispatcherThreadDelegate();

        void threadMain();

      private:
        DISALLOW_COPY_AND_ASSIGN(QueueDispatcherThreadDelegate);
    };

    /*
     * @brief Structure used in std algorithms to determine amount of request
     * during time scale
     */
    struct TimeScale {
      explicit TimeScale(const TimevalStruct& start, const TimevalStruct& end,
                         const int& connection_key)
      : start_(start),
        end_(end),
        connection_key_(connection_key) {};

      bool operator()(std::pair<RequestInfo*, TimevalStruct> mapEntry) {
        bool result = false;
        if (mapEntry.first->connectionID_ == connection_key_) {
          if (mapEntry.second.tv_sec >= start_.tv_sec &&
              mapEntry.second.tv_sec <= end_.tv_sec) {
            result = true;
          }
        }
        return result;
      };

      TimevalStruct start_;
      TimevalStruct end_;
      int connection_key_;
    };

    /*
     * @brief Structure used in std algorithms to determine amount of request
     * during time scale for application in defined hmi level
     */
    struct HMILevelTimeScale {
      explicit HMILevelTimeScale(
                          const TimevalStruct& start, const TimevalStruct& end,
                          const int& connection_key, const int& hmi_level)
      : start_(start),
        end_(end),
        connection_key_(connection_key),
        hmi_level_(hmi_level) {};

      bool operator()(std::pair<RequestInfo*, TimevalStruct> mapEntry) {
        bool result = false;
        if (mapEntry.first->connectionID_ == connection_key_ &&
            mapEntry.first->app_hmi_level_ == hmi_level_) {
          if (mapEntry.second.tv_sec >= start_.tv_sec &&
              mapEntry.second.tv_sec <= end_.tv_sec) {
            result = true;
          }
        }
        return result;
      };

      TimevalStruct start_;
      TimevalStruct end_;
      int connection_key_;
      int hmi_level_;
    };

    static const int                      DEFAULT_CYCLE_TIMEOUT = 250000;
    static log4cxx::LoggerPtr             logger_;
    std::list<WatchdogSubscriber*>        subscribers_;
    sync_primitives::Lock                 subscribersLock_;
    std::map<RequestInfo*, TimevalStruct> requests_;
    sync_primitives::Lock                 requestsLock_;
    threads::Thread                       queueDispatcherThread;

    DISALLOW_COPY_AND_ASSIGN(RequestWatchdog);
};

}  //  namespace request_watchdog

#endif  // SRC_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_WATCHDOG_H_
