/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "application_manager/request_info.h"
#include <iostream>
#include <vector>
#include <limits>
#include "gmock/gmock.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

namespace request_info = application_manager::request_controller;

namespace test {
namespace components {
namespace application_manager_test {

class MockRequest : public application_manager::commands::Command {
 public:
  MockRequest(uint32_t connection_key, uint32_t correlation_id)
      : connection_key_(connection_key), correlation_id_(correlation_id) {}
  MOCK_METHOD0(CheckPermissions, bool());
  MOCK_METHOD0(Init, bool());
  MOCK_METHOD0(Run, void());
  MOCK_METHOD0(CleanUp, bool());
  MOCK_CONST_METHOD0(default_timeout, uint32_t());
  MOCK_CONST_METHOD0(function_id, int32_t());
  MOCK_METHOD0(onTimeOut, void());
  MOCK_METHOD0(AllowedToTerminate, bool());
  MOCK_METHOD1(SetAllowedToTerminate, void(bool));

  uint32_t connection_key_;
  uint32_t correlation_id_;
  virtual uint32_t connection_key() const {
    return connection_key_;
  }
  virtual uint32_t correlation_id() const {
    return correlation_id_;
  }
};

class TestRequestInfo : public request_info::RequestInfo {
 public:
  TestRequestInfo(request_info::RequestPtr request,
                  const RequestType requst_type,
                  const TimevalStruct& start_time,
                  const uint64_t timeout_msec)
      : RequestInfo(request, requst_type, start_time, timeout_msec) {}
  void SetEndTime(const TimevalStruct& end_time) {
    end_time_ = end_time;
  }
};

class RequestInfoTest : public ::testing::Test {
 protected:
  virtual void SetUp() OVERRIDE {
    count_of_requests_for_test_ = 1000;
    hmi_connection_key_ = 0;
    mobile_connection_key1_ = 65431;
    mobile_connection_key2_ = 65123;
    mobile_correlation_id = 111;
    default_timeout_ = 10;
  }

  request_info::RequestInfoSet request_info_set_;
  uint32_t count_of_requests_for_test_;
  uint32_t hmi_connection_key_;
  uint32_t mobile_connection_key1_;
  uint32_t mobile_connection_key2_;
  uint32_t default_timeout_;
  uint32_t mobile_correlation_id;

  utils::SharedPtr<TestRequestInfo> CreateTestInfo(
      uint32_t connection_key,
      uint32_t correlation_id,
      request_info::RequestInfo::RequestType requst_type,
      const TimevalStruct& start_time,
      uint64_t timeout_msec) {
    utils::SharedPtr<MockRequest> mock_request =
        utils::MakeShared<MockRequest>(connection_key, correlation_id);
    utils::SharedPtr<TestRequestInfo> request =
        utils::MakeShared<TestRequestInfo>(
            mock_request, requst_type, start_time, timeout_msec);
    return request;
  }
};

TEST_F(RequestInfoTest, RequestInfoEqualEndTime) {
  std::vector<utils::SharedPtr<TestRequestInfo>> requests;
  const TimevalStruct& time = date_time::DateTime::getCurrentTime();
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    utils::SharedPtr<TestRequestInfo> request = CreateTestInfo(
        i, i, request_info::RequestInfo::MobileRequest, time, default_timeout_);
    request->SetEndTime(time);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.Size());
}

TEST_F(RequestInfoTest, AddRemoveHMIRequests) {
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    utils::SharedPtr<TestRequestInfo> request =
        CreateTestInfo(hmi_connection_key_,
                       i,
                       request_info::RequestInfo::HMIRequest,
                       date_time::DateTime::getCurrentTime(),
                       default_timeout_);
    EXPECT_TRUE(request_info_set_.Add(request));
    EXPECT_TRUE(request_info_set_.RemoveRequest(request));
  }

  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, AddHMIRequests_RemoveAllRequests) {
  std::vector<utils::SharedPtr<TestRequestInfo>> requests;

  // Add hmi requests
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    utils::SharedPtr<TestRequestInfo> request =
        CreateTestInfo(hmi_connection_key_,
                       i,
                       request_info::RequestInfo::HMIRequest,
                       date_time::DateTime::getCurrentTime(),
                       default_timeout_);
    requests.push_back(request);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.Size());

  // Delete every request
  std::vector<utils::SharedPtr<TestRequestInfo>>::iterator req_it =
      requests.begin();

  for (; req_it != requests.end(); ++req_it) {
    EXPECT_TRUE(request_info_set_.RemoveRequest(*req_it));
  }
  EXPECT_EQ(0u, request_info_set_.Size());

  // Delete requests by connection key
  req_it = requests.begin();
  for (; req_it != requests.end(); ++req_it) {
    EXPECT_TRUE(request_info_set_.Add(*req_it));
  }
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.Size());
  EXPECT_EQ(count_of_requests_for_test_,
            request_info_set_.RemoveByConnectionKey(hmi_connection_key_));
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, CheckRequestsMaxCount) {
  const uint32_t app_hmi_level_time_scale = 100;
  const uint32_t hmi_level_count = 1000;

  // Count of added requests is less than max possible
  std::vector<utils::SharedPtr<TestRequestInfo>> requests;
  for (uint32_t i = 0; i < hmi_level_count - 1; ++i) {
    utils::SharedPtr<TestRequestInfo> request =
        CreateTestInfo(mobile_connection_key1_,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       date_time::DateTime::getCurrentTime(),
                       default_timeout_);

    request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
    requests.push_back(request);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(hmi_level_count - 1, request_info_set_.Size());

  EXPECT_TRUE(request_info_set_.CheckHMILevelTimeScaleMaxRequest(
      mobile_apis::HMILevel::HMI_FULL,
      mobile_connection_key1_,
      app_hmi_level_time_scale,
      hmi_level_count));

  // Adding new request is correct
  utils::SharedPtr<TestRequestInfo> new_request =
      CreateTestInfo(mobile_connection_key1_,
                     hmi_level_count,
                     request_info::RequestInfo::MobileRequest,
                     date_time::DateTime::getCurrentTime(),
                     default_timeout_);
  new_request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_TRUE(request_info_set_.Add(new_request));
  EXPECT_EQ(hmi_level_count, request_info_set_.Size());

  // Count of added requests is max
  EXPECT_FALSE(request_info_set_.CheckHMILevelTimeScaleMaxRequest(
      mobile_apis::HMILevel::HMI_FULL,
      mobile_connection_key1_,
      app_hmi_level_time_scale,
      hmi_level_count));

  utils::SharedPtr<TestRequestInfo> new_request2 =
      CreateTestInfo(mobile_connection_key1_,
                     hmi_level_count + 1,
                     request_info::RequestInfo::MobileRequest,
                     date_time::DateTime::getCurrentTime(),
                     default_timeout_);

  EXPECT_TRUE(request_info_set_.Add(new_request2));
}

TEST_F(RequestInfoTest, CheckMaxCountOfRequest) {
  const uint32_t app_hmi_level_time_scale = 100;
  const uint32_t hmi_level_count = 1000;

  // Count of added requests is less than max possible
  std::vector<utils::SharedPtr<TestRequestInfo>> requests;
  for (uint32_t i = 0; i < hmi_level_count - 1; ++i) {
    utils::SharedPtr<TestRequestInfo> request =
        CreateTestInfo(mobile_connection_key1_,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       date_time::DateTime::getCurrentTime(),
                       default_timeout_);
    request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
    requests.push_back(request);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(hmi_level_count - 1, request_info_set_.Size());

  EXPECT_TRUE(request_info_set_.CheckTimeScaleMaxRequest(
      mobile_connection_key1_, app_hmi_level_time_scale, hmi_level_count));

  // Adding new request is correct
  utils::SharedPtr<TestRequestInfo> new_request =
      CreateTestInfo(mobile_connection_key1_,
                     hmi_level_count,
                     request_info::RequestInfo::MobileRequest,
                     date_time::DateTime::getCurrentTime(),
                     default_timeout_);
  new_request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_TRUE(request_info_set_.Add(new_request));
  EXPECT_EQ(hmi_level_count, request_info_set_.Size());

  // Count of added requests is max
  EXPECT_FALSE(request_info_set_.CheckTimeScaleMaxRequest(
      mobile_connection_key1_, app_hmi_level_time_scale, hmi_level_count));

  utils::SharedPtr<TestRequestInfo> new_request2 =
      CreateTestInfo(mobile_connection_key1_,
                     hmi_level_count + 1,
                     request_info::RequestInfo::MobileRequest,
                     date_time::DateTime::getCurrentTime(),
                     default_timeout_);

  EXPECT_TRUE(request_info_set_.Add(new_request2));
}

TEST_F(RequestInfoTest, AddMobileRequests_RemoveMobileRequests) {
  utils::SharedPtr<TestRequestInfo> mobile_request1 =
      CreateTestInfo(mobile_connection_key1_,
                     12345,
                     request_info::RequestInfo::MobileRequest,
                     date_time::DateTime::getCurrentTime(),
                     default_timeout_);
  EXPECT_TRUE(request_info_set_.Add(mobile_request1));
  utils::SharedPtr<TestRequestInfo> mobile_request2 =
      CreateTestInfo(mobile_connection_key2_,
                     54321,
                     request_info::RequestInfo::MobileRequest,
                     date_time::DateTime::getCurrentTime(),
                     default_timeout_);
  EXPECT_TRUE(request_info_set_.Add(mobile_request2));
  EXPECT_EQ(2u, request_info_set_.Size());
  EXPECT_EQ(2u, request_info_set_.RemoveMobileRequests());
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, AddMobileRequests_RemoveMobileRequestsByConnectionKey) {
  std::vector<utils::SharedPtr<TestRequestInfo>> requests;
  const uint32_t count_of_mobile_request1 = 200;
  const uint32_t count_of_mobile_request2 = 100;
  for (uint32_t i = 0; i < count_of_mobile_request1; ++i) {
    utils::SharedPtr<TestRequestInfo> mobile_request1 =
        CreateTestInfo(mobile_connection_key1_,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       date_time::DateTime::getCurrentTime(),
                       default_timeout_);

    requests.push_back(mobile_request1);
    EXPECT_TRUE(request_info_set_.Add(mobile_request1));
  }
  EXPECT_EQ(count_of_mobile_request1, request_info_set_.Size());

  for (uint32_t i = 0; i < count_of_mobile_request2; ++i) {
    utils::SharedPtr<TestRequestInfo> mobile_request2 =
        CreateTestInfo(mobile_connection_key2_,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       date_time::DateTime::getCurrentTime(),
                       default_timeout_);

    requests.push_back(mobile_request2);
    EXPECT_TRUE(request_info_set_.Add(mobile_request2));
  }
  EXPECT_EQ(count_of_mobile_request1 + count_of_mobile_request2,
            request_info_set_.Size());

  EXPECT_EQ(count_of_mobile_request1,
            request_info_set_.RemoveByConnectionKey(mobile_connection_key1_));
  EXPECT_EQ(count_of_mobile_request2,
            request_info_set_.RemoveByConnectionKey(mobile_connection_key2_));
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetFront) {
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    utils::SharedPtr<TestRequestInfo> request =
        CreateTestInfo(mobile_connection_key1_,
                       i,
                       request_info::RequestInfo::HMIRequest,
                       date_time::DateTime::getCurrentTime(),
                       i);
    request_info_set_.Add(request);
  }

  for (uint32_t i = 1; i < count_of_requests_for_test_; ++i) {
    request_info::RequestInfoPtr request_info = request_info_set_.Front();
    EXPECT_TRUE(request_info.valid());
    request_info = request_info_set_.FrontWithNotNullTimeout();
    EXPECT_TRUE(request_info.valid());
    EXPECT_TRUE(request_info_set_.RemoveRequest(request_info));
  }
  EXPECT_EQ(1u, request_info_set_.Size());
  EXPECT_EQ(1u,
            request_info_set_.RemoveByConnectionKey(mobile_connection_key1_));
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetFind) {
  std::vector<std::pair<uint32_t, uint32_t>> appid_connection_id;
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    appid_connection_id.push_back(
        std::pair<uint32_t, uint32_t>(i, count_of_requests_for_test_ - i));
  }
  std::vector<std::pair<uint32_t, uint32_t>>::iterator req_it =
      appid_connection_id.begin();
  const std::vector<std::pair<uint32_t, uint32_t>>::iterator end =
      appid_connection_id.end();

  for (; req_it != end; ++req_it) {
    utils::SharedPtr<TestRequestInfo> request =
        CreateTestInfo(req_it->first,
                       req_it->second,
                       request_info::RequestInfo::HMIRequest,
                       date_time::DateTime::getCurrentTime(),
                       10);
    EXPECT_TRUE(request_info_set_.Add(request));
  }

  request_info::RequestInfoPtr request = request_info_set_.Find(
      count_of_requests_for_test_, count_of_requests_for_test_);
  EXPECT_FALSE(request.valid());

  req_it = appid_connection_id.begin();
  for (; req_it != end; ++req_it) {
    request_info::RequestInfoPtr request =
        request_info_set_.Find(req_it->first, req_it->second);
    EXPECT_TRUE(request.valid());
    EXPECT_TRUE(request_info_set_.RemoveRequest(request));
    request = request_info_set_.Find(req_it->first, req_it->second);
    EXPECT_FALSE(request.valid());
  }
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetEqualHash) {
  request_info::RequestInfoSet request_info_set;
  const uint32_t connection_key = 65483;
  const uint32_t corr_id = 65483;
  utils::SharedPtr<TestRequestInfo> request =
      CreateTestInfo(connection_key,
                     corr_id,
                     request_info::RequestInfo::HMIRequest,
                     date_time::DateTime::getCurrentTime(),
                     10);
  EXPECT_TRUE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  EXPECT_EQ(1u, request_info_set.Size());
  request_info::RequestInfoPtr found =
      request_info_set.Find(connection_key, corr_id);
  EXPECT_TRUE(found.valid());
  EXPECT_TRUE(request_info_set.RemoveRequest(found));
  EXPECT_EQ(0u, request_info_set.Size());
  EXPECT_TRUE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  found = request_info_set.FrontWithNotNullTimeout();
  EXPECT_TRUE(found.valid());
  EXPECT_TRUE(request_info_set.RemoveRequest(found));
  found = request_info_set.FrontWithNotNullTimeout();
  EXPECT_FALSE(found.valid());
  found = request_info_set.Front();
  EXPECT_FALSE(found.valid());
  EXPECT_EQ(0u, request_info_set.Size());
}

TEST_F(RequestInfoTest, EndTimeisExpired) {
  TimevalStruct time = date_time::DateTime::getCurrentTime();

  TimevalStruct not_expired = date_time::DateTime::getCurrentTime();
  not_expired.tv_usec = std::numeric_limits<time_t>::min();

  TimevalStruct expired = date_time::DateTime::getCurrentTime();
  expired.tv_usec = std::numeric_limits<time_t>::max();

  utils::SharedPtr<TestRequestInfo> request =
      CreateTestInfo(mobile_connection_key1_,
                     mobile_correlation_id,
                     request_info::RequestInfo::MobileRequest,
                     time,
                     default_timeout_);

  request->SetEndTime(expired);
  EXPECT_FALSE(request->isExpired());

  request->SetEndTime(not_expired);
  EXPECT_TRUE(request->isExpired());
}

TEST_F(RequestInfoTest, UpdateEndTime) {
  TimevalStruct time = date_time::DateTime::getCurrentTime();
  utils::SharedPtr<TestRequestInfo> request =
      CreateTestInfo(mobile_connection_key1_,
                     mobile_correlation_id,
                     request_info::RequestInfo::MobileRequest,
                     time,
                     default_timeout_);
  request->SetEndTime(time);
  request->updateEndTime();
  TimevalStruct last_time = request->end_time();
  EXPECT_LE(time.tv_sec, last_time.tv_sec);
}

TEST_F(RequestInfoTest, UpdateTimeOut) {
  TimevalStruct time = date_time::DateTime::getCurrentTime();
  utils::SharedPtr<TestRequestInfo> request =
      CreateTestInfo(mobile_connection_key1_,
                     mobile_correlation_id,
                     request_info::RequestInfo::MobileRequest,
                     time,
                     default_timeout_);
  request->SetEndTime(time);
  request->updateEndTime();

  request->updateTimeOut(100);
  time = date_time::DateTime::getCurrentTime();
  TimevalStruct last_time = request->end_time();
  EXPECT_NEAR(time.tv_sec + 100, last_time.tv_sec, 500);
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
