/**
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

#include "application_manager/commands/command_request_impl.h"
#include "commands/hmi/request_to_hmi.h"

#include "utils/date_time.h"

namespace application_manager {

namespace request_controller {

  /*
   * @brief Typedef for active mobile request
   *
   */
  typedef utils::SharedPtr<commands::Command> RequestPtr;
  typedef utils::SharedPtr<commands::CommandRequestImpl> MobileRequestPtr;

  struct RequestInfo {
    RequestInfo(const uint64_t timeout_sec)
      : timeout_sec_(timeout_sec) {
        start_time_ = date_time::DateTime::getCurrentTime();
        updateEndTime();
      }

    RequestInfo(const TimevalStruct& start_time,const  uint64_t timeout_sec)
      : start_time_(start_time),
        timeout_sec_(timeout_sec) {
        updateEndTime();
    }

    virtual ~RequestInfo(){}

    virtual uint32_t requestId() = 0;
    virtual commands::Command* request() = 0;

    void updateEndTime() {
      end_time_ = date_time::DateTime::getCurrentTime();
      end_time_.tv_sec += timeout_sec_;

      // possible delay during IPC
      const uint32_t hmi_delay_sec = 1;
      end_time_.tv_sec += hmi_delay_sec;
    }

    void updateTimeOut(const uint64_t& timeout_sec) {
      timeout_sec_ = timeout_sec;
      updateEndTime();
    }

    bool isExpired() {
      if ( date_time::GREATER ==
           date_time::DateTime::compareTime(end_time_, date_time::DateTime::getCurrentTime()) ) {
        return false;
      }
      return true;
    }

    TimevalStruct start_time() {
      return start_time_;
    }

    uint64_t timeout_sec() {
      return timeout_sec_;
    }

    TimevalStruct end_time() {
      return end_time_;
    }

    uint32_t app_id() {
      return app_id_;
    }

    mobile_apis::HMILevel::eType hmi_level() {
      return hmi_level_;
    }

  protected:
    TimevalStruct                 start_time_;
    uint64_t                      timeout_sec_;
    TimevalStruct                 end_time_;
    uint32_t                      app_id_;
    mobile_apis::HMILevel::eType  hmi_level_;
  };

  typedef utils::SharedPtr<RequestInfo> RequestInfoPtr;

  struct RequestInfoComparator {
      bool operator() (const RequestInfoPtr lhs,
                       const RequestInfoPtr rhs) const {
        date_time::TimeCompare compare_result =
            date_time::DateTime::compareTime(lhs->end_time(), rhs->end_time());

        return compare_result == date_time::LESS;
      }
  };

  typedef std::set<RequestInfoPtr,RequestInfoComparator> RequestInfoSet;

  struct HMIRequestInfo: public RequestInfo {
    HMIRequestInfo(RequestPtr request, const uint64_t timeout_sec);
    HMIRequestInfo(RequestPtr request, const TimevalStruct& start_time,
                     const  uint64_t timeout_sec);

    RequestPtr request_;
    uint32_t correlation_id_;

    virtual uint32_t requestId() {
      return correlation_id_;
    }

    virtual commands::Command* request() {
      return request_.get();
    }
  };

  struct MobileRequestInfo: public RequestInfo {
    MobileRequestInfo(RequestPtr request,
                      const uint64_t timeout_sec);

    MobileRequestInfo(RequestPtr request,
                      const TimevalStruct& start_time,
                      const uint64_t timeout_sec);

    RequestPtr request_;
    uint32_t mobile_correlation_id_;

    virtual uint32_t requestId() {
      return mobile_correlation_id_;
    }

    virtual commands::Command* request() {
      return request_.get();
    }
  };

  /**
  * @brief Structure used in std algorithms to determine amount of request
  * during time scale
  */
  struct TimeScale {
    TimeScale(const TimevalStruct& start,
              const TimevalStruct& end,
              const uint32_t& app_id)
      : start_(start),
        end_(end),
        app_id_(app_id) {}

    bool operator()(RequestInfoPtr setEntry) {

      if (!setEntry.valid()) {
        return false;
      }

      if (setEntry->app_id() != app_id_) {
        return false;
      }

      if (date_time::DateTime::getmSecs(setEntry->start_time())
          < date_time::DateTime::getmSecs(start_) ||
          date_time::DateTime::getmSecs(setEntry->start_time())
          > date_time::DateTime::getmSecs(end_)) {
        return false;
      }

      return true;
    }

  private:
    TimevalStruct  start_;
    TimevalStruct  end_;
    uint32_t       app_id_;
  };

  /**
  * @brief Structure used in std algorithms to determine amount of request
  * during time scale for application in defined hmi level
  */
  struct HMILevelTimeScale {
    HMILevelTimeScale(const TimevalStruct& start,
                      const TimevalStruct& end,
                      const uint32_t& app_id,
                      const mobile_apis::HMILevel::eType& hmi_level)
      : start_(start),
        end_(end),
        app_id_(app_id),
        hmi_level_(hmi_level) {}

    bool operator()(RequestInfoPtr setEntry) {
      if (!setEntry.valid()) {
        return false;
      }

      if (setEntry->app_id() != app_id_) {
        return false;
      }

      if (setEntry->hmi_level() != hmi_level_) {
        return false;
      }

      if (date_time::DateTime::getSecs(setEntry->start_time())
          < date_time::DateTime::getSecs(start_) ||
          date_time::DateTime::getSecs(setEntry->start_time())
          > date_time::DateTime::getSecs(end_)) {
        return false;
      }

      return true;
    }
  private:
    TimevalStruct                start_;
    TimevalStruct                end_;
    uint32_t                     app_id_;
    mobile_apis::HMILevel::eType hmi_level_;
  };



}  //  namespace request_controller

} //  namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_INFO_H_
