/*
  * Copyright (c) 2017, Ford Motor Company
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
#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_TRACKER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_TRACKER_H_

#include <string>
#include <map>
#include <vector>
#include "application_manager/request_controller_settings.h"
#include "interfaces/MOBILE_API.h"
#include "utils/date_time.h"

namespace application_manager {

namespace request_controller {

/**
 * @brief The TrackResult enum defines results of application request tracking
 */
enum class TrackResult {
  kSuccess,
  kNoneLevelMaxRequestsExceeded,
  kMaxRequestsExceeded
};

/**
 * @brief The RequestTracker class tracks requests per time constraints.
 * There are several parameters in configuration file defining maximum
 * requests
 * number and time scale for that number, so application must consider that,
 * otherwise it will be disconnected and won't be registered till next
 * ignition cycle.
 */
class RequestTracker {
 public:
  typedef uint32_t ApplicationID;

  /**
   * @brief RequestTracker class constructor
   * @param settings Settings instance having time scale and maximum requests
   * values
   */
  explicit RequestTracker(const RequestControlerSettings& settings);

  /**
   * @brief Tracks amount of requests per defined time considering HMI level
   * Currently there is a separate restrictions can be set for NONE level.
   * Other levels tracked by single constraint.
   * @param app_id Unique application id
   * @param level HMI level of request
   * @return Success if constraints are not exceeded, otherwise - exceeded
   * error code (depends on HMI level)
   */
  TrackResult Track(const ApplicationID& app_id,
                    const mobile_apis::HMILevel::eType level);

 private:
  typedef std::vector<TimevalStruct> RequestAddedAt;
  typedef std::map<ApplicationID, RequestAddedAt> ApplicationsRequestsTracker;

  /**
   * @brief Checks whether maximum requests number is exceeded per defined
   * time
   * scale.
   * @param app_id Unique application id
   * @param time_scale Time scale defined in configuration file
   * @param max_requests Maximum requests number defined in configuration file
   * @param tracker Container tracking applications requests amount and their
   * time of addition into processing
   * @return
   */
  bool Track(const ApplicationID& app_id,
             const uint32_t time_scale,
             const uint32_t max_requests,
             ApplicationsRequestsTracker& tracker);

  /**
   * @brief settings_ having time scale and maximum requests values
   */
  const RequestControlerSettings& settings_;

  /**
   * @brief Tracker for applications requests done in NONE level
   */
  ApplicationsRequestsTracker none_level_tracker_;

  /**
   * @brief Tracker for applications requests done in other than NONE level
   */
  ApplicationsRequestsTracker tracker_;
};

}  //  namespace request_controller
}  //  namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_REQUEST_TRACKER_H_
