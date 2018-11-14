/*
 * \file request_info.h
 * \brief request information structure header file.
 *
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_INFO_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_INFO_H_

#include <stdint.h>
#include <set>

#include "application_manager/commands/command_request_impl.h"
#include "commands/request_to_hmi.h"

#include "utils/date_time.h"

namespace application_manager {

namespace request_controller {

/*
 * @brief Typedef for active mobile request
 *
 */
typedef std::shared_ptr<commands::Command> RequestPtr;

struct RequestInfo {
  enum RequestType { RequestNone, MobileRequest, HMIRequest };

  RequestInfo()
      : timeout_msec_(0)
      , app_id_(0)
      , request_type_(RequestNone)
      , correlation_id_(0) {
    start_time_ = date_time::getCurrentTime();
    updateEndTime();
  }
  virtual ~RequestInfo() {}

  RequestInfo(RequestPtr request,
              const RequestType request_type,
              const uint64_t timeout_msec)
      : request_(request)
      , timeout_msec_(timeout_msec)
      , app_id_(0)
      , correlation_id_(0) {
    start_time_ = date_time::getCurrentTime();
    updateEndTime();
    request_type_ = request_type;
  }

  RequestInfo(RequestPtr request,
              const RequestType request_type,
              const date_time::TimeDuration& start_time,
              const uint64_t timeout_msec);

  void updateEndTime();

  void updateTimeOut(const uint64_t& timeout_msec);

  bool isExpired();

  date_time::TimeDuration start_time() {
    return start_time_;
  }

  void update_start_time(date_time::TimeDuration start_time) {
    start_time_ = start_time;
  }

  uint64_t timeout_msec() {
    return timeout_msec_;
  }

  void set_timeout_msec(uint64_t timeout) {
    timeout_msec_ = timeout;
  }

  date_time::TimeDuration end_time() {
    return end_time_;
  }

  uint32_t app_id() {
    return app_id_;
  }

  RequestType request_type() const {
    return request_type_;
  }

  uint32_t requestId() {
    return correlation_id_;
  }

  commands::Command* request() {
    return request_.get();
  }
  uint64_t hash();
  static uint64_t GenerateHash(uint32_t var1, uint32_t var2);
  static uint32_t HmiConnectionKey;

 protected:
  RequestPtr request_;
  date_time::TimeDuration start_time_;
  uint64_t timeout_msec_;
  date_time::TimeDuration end_time_;
  uint32_t app_id_;
  RequestType request_type_;
  uint32_t correlation_id_;
};

typedef std::shared_ptr<RequestInfo> RequestInfoPtr;

struct MobileRequestInfo : public RequestInfo {
  MobileRequestInfo(RequestPtr request, const uint64_t timeout_msec);
  MobileRequestInfo(RequestPtr request,
                    const date_time::TimeDuration& start_time,
                    const uint64_t timeout_msec);
};

struct HMIRequestInfo : public RequestInfo {
  HMIRequestInfo(RequestPtr request, const uint64_t timeout_msec);
  HMIRequestInfo(RequestPtr request,
                 const date_time::TimeDuration& start_time,
                 const uint64_t timeout_msec);
};

// Request info, for searching in request info set by log_n time
// Returns correct hash by app_id and corr_id
struct FakeRequestInfo : public RequestInfo {
  FakeRequestInfo(uint32_t app_id, uint32_t correaltion_id);
};

struct RequestInfoTimeComparator {
  bool operator()(const RequestInfoPtr lhs, const RequestInfoPtr rhs) const;
};

struct RequestInfoHashComparator {
  bool operator()(const RequestInfoPtr lhs, const RequestInfoPtr rhs) const;
};

typedef std::set<RequestInfoPtr, RequestInfoTimeComparator>
    TimeSortedRequestInfoSet;
typedef std::set<RequestInfoPtr, RequestInfoHashComparator>
    HashSortedRequestInfoSet;

/*
 * @brief RequestInfoSet provides uniue requests bu corralation_id and app_id
 *
 */
class RequestInfoSet {
 public:
  /*
   * @brief Add requests into colletion by log(n) time
   * @param request_info - request to add
   * @return false is request with the same app_id and correlation_id exist
   */
  bool Add(RequestInfoPtr request_info);

  /*
   * @brief Find requests int colletion by log(n) time
   * @param connection_key - connection_key of request
   * @param correlation_id - correlation_id of request
   * @return founded request or shared_ptr with NULL
   */
  RequestInfoPtr Find(const uint32_t connection_key,
                      const uint32_t correlation_id);

  /*
   * @brief Get request with smalest end_time_
   * @return founded request or shared_ptr with NULL
   */
  RequestInfoPtr Front();

  /*
   * @brief Get request with smalest end_time_ != 0
   * @return founded request or shared_ptr with NULL
   */
  RequestInfoPtr FrontWithNotNullTimeout();

  /*
   * @brief Erase request from colletion by log(n) time
   * @param request_info - request to erase
   * @return true if Erase succes, otherwise return false
   */
  bool RemoveRequest(const RequestInfoPtr request_info);

  /*
   * @brief Erase request from colletion by connection_key
   * @param connection_key - connection_key of requests to erase
   * @return count of erased requests
   */
  uint32_t RemoveByConnectionKey(uint32_t connection_key);

  /*
   * @brief Erase all mobile requests from controller
   * @return count of erased requests
   */
  uint32_t RemoveMobileRequests();

  /*
   * @return count of requestd in collections
   */
  const size_t Size();

 private:
  /*
   * @brief Comparator of connection key for std::find_if function
   */
  struct AppIdCompararator {
    enum CompareType { Equal, NotEqual };
    AppIdCompararator(CompareType compare_type, uint32_t app_id)
        : app_id_(app_id), compare_type_(compare_type) {}
    bool operator()(const RequestInfoPtr value_compare) const;

   private:
    uint32_t app_id_;
    CompareType compare_type_;
  };

  bool Erase(const RequestInfoPtr request_info);

  /*
   * @brief Erase requests from collection if filter allows
   * @param filter - filtering predicate
   * @return count of erased requests
   */
  uint32_t RemoveRequests(const RequestInfoSet::AppIdCompararator& filter);

  /*
   * @brief Debug function, will raise assert if set sizes are noit equal
   */
  inline void CheckSetSizes();
  TimeSortedRequestInfoSet time_sorted_pending_requests_;
  HashSortedRequestInfoSet hash_sorted_pending_requests_;

  sync_primitives::Lock pending_requests_lock_;
};

/**
* @brief Structure used in std algorithms to determine amount of request
* during time scale
*/
struct TimeScale {
  TimeScale(const date_time::TimeDuration& start,
            const date_time::TimeDuration& end,
            const uint32_t& app_id)
      : start_(start), end_(end), app_id_(app_id) {}

  bool operator()(RequestInfoPtr setEntry) {
    if (setEntry.use_count() == 0) {
      return false;
    }

    if (setEntry->app_id() != app_id_) {
      return false;
    }

    if ((setEntry->start_time() < start_) || (end_ < setEntry->start_time())) {
      return false;
    }

    return true;
  }

 private:
  date_time::TimeDuration start_;
  date_time::TimeDuration end_;
  uint32_t app_id_;
};

}  //  namespace request_controller

}  //  namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_INFO_H_
