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

#include "gtest/gtest.h"
#include "utils/macro.h"
#include "application_manager/request_tracker.h"
#include "application_manager/mock_request_controller_settings.h"
#include "interfaces/MOBILE_API.h"
#include "utils/lock.h"
#include "utils/conditional_variable.h"

namespace test {
namespace components {
namespace request_controller_test {

using ::testing::Return;
using ::testing::ReturnRef;

class RequestTrackerTestClass : public ::testing::Test {
 public:
  RequestTrackerTestClass() : tracker_(mock_request_controller_settings_) {}

  void SetDefaultConstraints() {
    EXPECT_CALL(mock_request_controller_settings_,
                app_hmi_level_none_time_scale())
        .WillRepeatedly(ReturnRef(kDefaultAppHmiLevelNoneRequestsTimeScale));

    EXPECT_CALL(mock_request_controller_settings_,
                app_hmi_level_none_time_scale_max_requests())
        .WillRepeatedly(ReturnRef(kDefaultAppHmiLevelNoneTimeScaleMaxRequests));

    EXPECT_CALL(mock_request_controller_settings_, app_time_scale())
        .WillRepeatedly(ReturnRef(kDefaultAppRequestsTimeScale));

    EXPECT_CALL(mock_request_controller_settings_,
                app_time_scale_max_requests())
        .WillRepeatedly(ReturnRef(kDefaultAppTimeScaleMaxRequests));
  }

 protected:
  application_manager_test::MockRequestControlerSettings
      mock_request_controller_settings_;

  application_manager::request_controller::RequestTracker tracker_;

  const uint32_t kDefaultAppHmiLevelNoneRequestsTimeScale = 10u;
  const uint32_t kDefaultAppHmiLevelNoneTimeScaleMaxRequests = 100u;
  const uint32_t kDefaultAppTimeScaleMaxRequests = 5u;
  const uint32_t kDefaultAppRequestsTimeScale = 200u;
};

TEST_F(RequestTrackerTestClass, TrackAppRequestInNone_ExpectSuccessTillLimit) {
  const uint32_t app_id = 1u;
  const mobile_apis::HMILevel::eType none_level =
      mobile_apis::HMILevel::HMI_NONE;

  SetDefaultConstraints();

  for (uint32_t i = 0; i < kDefaultAppHmiLevelNoneTimeScaleMaxRequests; ++i) {
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id, none_level));
  }

  EXPECT_EQ(application_manager::request_controller::TrackResult::
                kNoneLevelMaxRequestsExceeded,
            tracker_.Track(app_id, none_level));
}

TEST_F(RequestTrackerTestClass,
       TrackAppRequestInNone_NoLimits_ExpectAlwaysSuccess) {
  const uint32_t no_limit = 0;

  EXPECT_CALL(mock_request_controller_settings_,
              app_hmi_level_none_time_scale())
      .WillRepeatedly(ReturnRef(no_limit));

  EXPECT_CALL(mock_request_controller_settings_,
              app_hmi_level_none_time_scale_max_requests())
      .WillRepeatedly(ReturnRef(no_limit));

  const uint32_t app_id = 1u;
  const mobile_apis::HMILevel::eType none_level =
      mobile_apis::HMILevel::HMI_NONE;

  for (uint32_t i = 0; i < kDefaultAppHmiLevelNoneTimeScaleMaxRequests; ++i) {
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id, none_level));
  }

  EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
            tracker_.Track(app_id, none_level));
}

TEST_F(RequestTrackerTestClass,
       TrackAppRequestInOtherThanNone_ExpectSuccessTillLimit) {
  const uint32_t app_id = 1u;
  mobile_apis::HMILevel::eType hmi_level =
      mobile_apis::HMILevel::HMI_BACKGROUND;

  SetDefaultConstraints();

  for (uint32_t i = 0; i < kDefaultAppTimeScaleMaxRequests; ++i) {
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id, hmi_level));
    if (i % 2) {
      hmi_level = mobile_apis::HMILevel::HMI_FULL;
    } else {
      hmi_level = mobile_apis::HMILevel::HMI_LIMITED;
    }
  }

  EXPECT_EQ(application_manager::request_controller::TrackResult::
                kMaxRequestsExceeded,
            tracker_.Track(app_id, hmi_level));
}

TEST_F(RequestTrackerTestClass,
       TrackAppRequestInOtherThanNone_NoLimits_ExpectAlwaysSuccess) {
  const uint32_t no_limit = 0;

  EXPECT_CALL(mock_request_controller_settings_, app_time_scale())
      .WillRepeatedly(ReturnRef(no_limit));

  EXPECT_CALL(mock_request_controller_settings_, app_time_scale_max_requests())
      .WillRepeatedly(ReturnRef(no_limit));

  const uint32_t app_id = 1u;
  mobile_apis::HMILevel::eType hmi_level =
      mobile_apis::HMILevel::HMI_BACKGROUND;

  for (uint32_t i = 0; i < kDefaultAppTimeScaleMaxRequests; ++i) {
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id, hmi_level));
    if (i % 2) {
      hmi_level = mobile_apis::HMILevel::HMI_FULL;
    } else {
      hmi_level = mobile_apis::HMILevel::HMI_LIMITED;
    }
  }

  EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
            tracker_.Track(app_id, hmi_level));
}

TEST_F(RequestTrackerTestClass,
       TrackTwoAppsRequestInNone_ExpectSuccessTillLimit) {
  const uint32_t app_id_1 = 1u;
  const uint32_t app_id_2 = 2u;

  const mobile_apis::HMILevel::eType none_level =
      mobile_apis::HMILevel::HMI_NONE;

  SetDefaultConstraints();

  for (uint32_t i = 0; i < kDefaultAppHmiLevelNoneTimeScaleMaxRequests; ++i) {
    if (i % 2) {
      EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
                tracker_.Track(app_id_1, none_level));
    } else {
      EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
                tracker_.Track(app_id_2, none_level));
    }
  }

  for (uint32_t i = 0; i < kDefaultAppHmiLevelNoneTimeScaleMaxRequests / 2;
       ++i) {
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id_1, none_level));
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id_2, none_level));
  }

  EXPECT_EQ(application_manager::request_controller::TrackResult::
                kNoneLevelMaxRequestsExceeded,
            tracker_.Track(app_id_1, none_level));

  EXPECT_EQ(application_manager::request_controller::TrackResult::
                kNoneLevelMaxRequestsExceeded,
            tracker_.Track(app_id_2, none_level));
}

TEST_F(RequestTrackerTestClass,
       TrackAppRequestInNone_DoPause_TrackAgain_ExpectSuccessTillLimit) {
  const uint32_t max_requests = 5;
  const uint32_t time_scale_ms = 1;

  sync_primitives::ConditionalVariable awaiter;
  sync_primitives::Lock lock;
  sync_primitives::AutoLock auto_lock(lock);

  EXPECT_CALL(mock_request_controller_settings_,
              app_hmi_level_none_time_scale())
      .WillRepeatedly(ReturnRef(time_scale_ms));

  EXPECT_CALL(mock_request_controller_settings_,
              app_hmi_level_none_time_scale_max_requests())
      .WillRepeatedly(ReturnRef(max_requests));

  const uint32_t app_id = 1u;
  const mobile_apis::HMILevel::eType none_level =
      mobile_apis::HMILevel::HMI_NONE;

  for (uint32_t i = 0; i < max_requests; ++i) {
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id, none_level));
  }

  awaiter.WaitFor(auto_lock, time_scale_ms * 2);

  for (uint32_t i = 0; i < max_requests; ++i) {
    EXPECT_EQ(application_manager::request_controller::TrackResult::kSuccess,
              tracker_.Track(app_id, none_level));
  }

  EXPECT_EQ(application_manager::request_controller::TrackResult::
                kNoneLevelMaxRequestsExceeded,
            tracker_.Track(app_id, none_level));
}

}  // namespace request_controller
}  // namespace components
}  // namespace test
