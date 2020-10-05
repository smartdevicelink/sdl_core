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
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "application_manager/request_tracker.h"
#include "application_manager/message_helper.h"
#include "utils/logger.h"
#include "utils/macro.h"

namespace application_manager {

namespace request_controller {

SDL_CREATE_LOG_VARIABLE("RequestController")

RequestTracker::RequestTracker(const RequestControlerSettings& settings)
    : settings_(settings) {}

TrackResult RequestTracker::Track(const ApplicationID& app_id,
                                  const mobile_apis::HMILevel::eType level) {
  SDL_LOG_AUTO_TRACE();
  bool track_result = false;

  SDL_LOG_DEBUG("Tracking request for level: " << EnumToString(level));

  if (mobile_apis::HMILevel::HMI_NONE == level) {
    track_result = Track(app_id,
                         settings_.app_hmi_level_none_time_scale(),
                         settings_.app_hmi_level_none_time_scale_max_requests(),
                         none_level_tracker_);

    return track_result ? TrackResult::kSuccess
                        : TrackResult::kNoneLevelMaxRequestsExceeded;
  }

  track_result = Track(app_id,
                       settings_.app_time_scale(),
                       settings_.app_time_scale_max_requests(),
                       tracker_);

  return track_result ? TrackResult::kSuccess
                      : TrackResult::kMaxRequestsExceeded;
}

bool RequestTracker::Track(const ApplicationID& app_id,
                           const uint32_t time_scale,
                           const uint32_t max_requests,
                           ApplicationsRequestsTracker& tracker) {
  SDL_LOG_AUTO_TRACE();
  using namespace date_time;

  if (!time_scale || !max_requests) {
    SDL_LOG_INFO("Time scale request tracking is disabled.");
    return true;
  }

  SDL_LOG_DEBUG("Time scale is: " << time_scale << ". Max requests number is: "
                                  << max_requests);

  SDL_LOG_DEBUG("Tracking app id: " << app_id);
  ApplicationsRequestsTracker::iterator it_app = tracker.find(app_id);

  if (tracker.end() == it_app) {
    SDL_LOG_DEBUG("Adding new application into tracking.");
    tracker[app_id].push_back(getCurrentTime());
    return true;
  }

  SDL_LOG_DEBUG("Amount of known requests is: " << it_app->second.size());

  if (it_app->second.size() < max_requests) {
    SDL_LOG_DEBUG("Adding new request into tracking.");
    tracker[app_id].push_back(getCurrentTime());
    return true;
  }

  SDL_LOG_DEBUG("Oldest request is added at: "
                << getmSecs(it_app->second.front())
                << ". Current time is: " << getmSecs(getCurrentTime())
                << ". Time scale is: " << time_scale);

  if (calculateTimeSpan(it_app->second.front()) > time_scale) {
    SDL_LOG_DEBUG("Dropping oldest request, adding new one.");
    ApplicationsRequestsTracker::mapped_type& times = tracker[app_id];

    DCHECK_OR_RETURN(!times.empty(), false);

    times.erase(times.begin());
    times.push_back(getCurrentTime());
    return true;
  }

  SDL_LOG_DEBUG("Requests amount per time scale is exceeded.");

  return false;
}

}  // namespace request_controller
}  // namespace application_manager
