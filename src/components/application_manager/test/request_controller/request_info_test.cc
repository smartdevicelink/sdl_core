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
#include <stdint.h>
#include <iostream>
#include <vector>
#include <limits>
#include "gmock/gmock.h"
#include "application_manager/test_request_info.h"
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

namespace {
const uint32_t kCountOfRequestsForTest = 1000u;
const uint32_t kHmiConnectionKey = 0u;
const uint32_t kMobileConnectionKey1 = 65431u;
const uint32_t kMobileConnectionKey2 = 65123u;
const uint32_t kDefaultTimeout = 10u;
}  // namespace

class RequestInfoTest : public ::testing::Test {
 protected:
  request_info::RequestInfoSet request_info_set_;

  uint32_t mobile_correlation_id;

  utils::SharedPtr<RequestInfoForTesting> CreateTestInfo(
      const uint32_t connection_key,
      const uint32_t correlation_id,
      const request_info::RequestInfo::RequestType request_type,
      const uint64_t timeout_sec) {
    utils::SharedPtr<MockRequest> mock_request =
        utils::MakeShared<MockRequest>(connection_key, correlation_id);
    utils::SharedPtr<RequestInfoForTesting> request =
        utils::MakeShared<RequestInfoForTesting>(mock_request,
                                                 request_type,
                                                 timeout_sec,
                                                 connection_key,
                                                 correlation_id);

    return request;
  }
};

TEST_F(RequestInfoTest, RequestInfoEqualEndTime) {
  const TimevalStruct& end_time = date_time::DateTime::getCurrentTime();
  for (uint32_t i = 0; i < kCountOfRequestsForTest; ++i) {
    utils::SharedPtr<RequestInfoForTesting> request = CreateTestInfo(
        i, i, request_info::RequestInfo::MobileRequest, kDefaultTimeout);
    request->SetEndTime(end_time);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(kCountOfRequestsForTest, request_info_set_.Size());
}

TEST_F(RequestInfoTest, AddRemoveHMIRequests) {
  for (uint32_t i = 0; i < kCountOfRequestsForTest; ++i) {
    utils::SharedPtr<RequestInfoForTesting> request =
        CreateTestInfo(kHmiConnectionKey,
                       i,
                       request_info::RequestInfo::HMIRequest,
                       kDefaultTimeout);
    EXPECT_TRUE(request_info_set_.Add(request));
    EXPECT_TRUE(request_info_set_.RemoveRequest(request));
  }

  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, AddHMIRequests_RemoveAllRequests) {
  std::vector<utils::SharedPtr<RequestInfoForTesting>> requests;

  // Add hmi requests
  for (uint32_t i = 0; i < kCountOfRequestsForTest; ++i) {
    utils::SharedPtr<RequestInfoForTesting> request =
        CreateTestInfo(kHmiConnectionKey,
                       i,
                       request_info::RequestInfo::HMIRequest,
                       kDefaultTimeout);
    requests.push_back(request);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(kCountOfRequestsForTest, request_info_set_.Size());

  // Delete every request
  std::vector<utils::SharedPtr<RequestInfoForTesting>>::iterator req_it =
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
  EXPECT_EQ(kCountOfRequestsForTest, request_info_set_.Size());
  EXPECT_EQ(kCountOfRequestsForTest,
            request_info_set_.RemoveByConnectionKey(kHmiConnectionKey));
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, CheckRequestsMaxCount) {
  const uint32_t kAppHmiLevelTimeScale = 100u;
  const uint32_t kHmiLevelCount = 1000u;

  // Count of added requests is less than max possible
  std::vector<utils::SharedPtr<RequestInfoForTesting>> requests;
  for (uint32_t i = 0; i < kHmiLevelCount - 1; ++i) {
    utils::SharedPtr<RequestInfoForTesting> request =
        CreateTestInfo(kMobileConnectionKey1,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       kDefaultTimeout);

    request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
    requests.push_back(request);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(kHmiLevelCount - 1, request_info_set_.Size());

  EXPECT_TRUE(request_info_set_.CheckHMILevelTimeScaleMaxRequest(
      mobile_apis::HMILevel::HMI_FULL,
      kMobileConnectionKey1,
      kAppHmiLevelTimeScale,
      kHmiLevelCount));

  // Adding new request is correct
  utils::SharedPtr<RequestInfoForTesting> new_request =
      CreateTestInfo(kMobileConnectionKey1,
                     kHmiLevelCount,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);
  new_request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_TRUE(request_info_set_.Add(new_request));
  EXPECT_EQ(kHmiLevelCount, request_info_set_.Size());

  // Count of added requests is max
  EXPECT_FALSE(request_info_set_.CheckHMILevelTimeScaleMaxRequest(
      mobile_apis::HMILevel::HMI_FULL,
      kMobileConnectionKey1,
      kAppHmiLevelTimeScale,
      kHmiLevelCount));

  utils::SharedPtr<RequestInfoForTesting> new_request2 =
      CreateTestInfo(kMobileConnectionKey1,
                     kHmiLevelCount + 1,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);
  EXPECT_TRUE(request_info_set_.Add(new_request2));
}

TEST_F(RequestInfoTest, CheckMaxCountOfRequest) {
  const uint32_t kAppHmiLevelTimeScale = 100u;
  const uint32_t kHmiLevelCount = 1000u;

  // Count of added requests is less than max possible
  std::vector<utils::SharedPtr<RequestInfoForTesting>> requests;
  for (uint32_t i = 0; i < kHmiLevelCount - 1; ++i) {
    utils::SharedPtr<RequestInfoForTesting> request =
        CreateTestInfo(kMobileConnectionKey1,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       kDefaultTimeout);
    request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
    requests.push_back(request);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(kHmiLevelCount - 1, request_info_set_.Size());

  EXPECT_TRUE(request_info_set_.CheckTimeScaleMaxRequest(
      kMobileConnectionKey1, kAppHmiLevelTimeScale, kHmiLevelCount));

  // Adding new request is correct
  utils::SharedPtr<RequestInfoForTesting> new_request =
      CreateTestInfo(kMobileConnectionKey1,
                     kHmiLevelCount,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);
  new_request->set_hmi_level(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_TRUE(request_info_set_.Add(new_request));
  EXPECT_EQ(kHmiLevelCount, request_info_set_.Size());

  // Count of added requests is max
  EXPECT_FALSE(request_info_set_.CheckTimeScaleMaxRequest(
      kMobileConnectionKey1, kAppHmiLevelTimeScale, kHmiLevelCount));

  utils::SharedPtr<RequestInfoForTesting> new_request2 =
      CreateTestInfo(kMobileConnectionKey1,
                     kHmiLevelCount + 1,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);
  EXPECT_TRUE(request_info_set_.Add(new_request2));
}

TEST_F(RequestInfoTest, AddMobileRequests_RemoveMobileRequests) {
  utils::SharedPtr<RequestInfoForTesting> mobile_request1 =
      CreateTestInfo(kMobileConnectionKey1,
                     12345,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);
  EXPECT_TRUE(request_info_set_.Add(mobile_request1));
  utils::SharedPtr<RequestInfoForTesting> mobile_request2 =
      CreateTestInfo(kMobileConnectionKey2,
                     54321,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);
  EXPECT_TRUE(request_info_set_.Add(mobile_request2));
  EXPECT_EQ(2u, request_info_set_.Size());
  EXPECT_EQ(2u, request_info_set_.RemoveMobileRequests());
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, AddMobileRequests_RemoveMobileRequestsByConnectionKey) {
  std::vector<utils::SharedPtr<RequestInfoForTesting>> requests;
  const uint32_t kCountOfMobileequest1 = 200u;
  const uint32_t kCountOfMobileequest2 = 100u;
  for (uint32_t i = 0; i < kCountOfMobileequest1; ++i) {
    utils::SharedPtr<RequestInfoForTesting> mobile_request1 =
        CreateTestInfo(kMobileConnectionKey1,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       kDefaultTimeout);

    requests.push_back(mobile_request1);
    EXPECT_TRUE(request_info_set_.Add(mobile_request1));
  }
  EXPECT_EQ(kCountOfMobileequest1, request_info_set_.Size());

  for (uint32_t i = 0; i < kCountOfMobileequest2; ++i) {
    utils::SharedPtr<RequestInfoForTesting> mobile_request2 =
        CreateTestInfo(kMobileConnectionKey2,
                       i,
                       request_info::RequestInfo::MobileRequest,
                       kDefaultTimeout);

    requests.push_back(mobile_request2);
    EXPECT_TRUE(request_info_set_.Add(mobile_request2));
  }
  EXPECT_EQ(kCountOfMobileequest1 + kCountOfMobileequest2,
            request_info_set_.Size());

  EXPECT_EQ(kCountOfMobileequest1,
            request_info_set_.RemoveByConnectionKey(kMobileConnectionKey1));
  EXPECT_EQ(kCountOfMobileequest2,
            request_info_set_.RemoveByConnectionKey(kMobileConnectionKey2));
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetFront) {
  for (uint32_t i = 0; i < kCountOfRequestsForTest; ++i) {
    utils::SharedPtr<RequestInfoForTesting> request = CreateTestInfo(
        kMobileConnectionKey1, i, request_info::RequestInfo::HMIRequest, i);
    request_info_set_.Add(request);
  }

  for (uint32_t i = 1; i < kCountOfRequestsForTest; ++i) {
    request_info::RequestInfoPtr request_info = request_info_set_.Front();
    EXPECT_TRUE(request_info.valid());
    request_info = request_info_set_.FrontWithNotNullTimeout();
    EXPECT_TRUE(request_info.valid());
    EXPECT_TRUE(request_info_set_.RemoveRequest(request_info));
  }
  EXPECT_EQ(1u, request_info_set_.Size());
  EXPECT_EQ(1u, request_info_set_.RemoveByConnectionKey(kMobileConnectionKey1));
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetFind) {
  std::vector<std::pair<uint32_t, uint32_t>> appid_connection_id;
  for (uint32_t i = 0; i < kCountOfRequestsForTest; ++i) {
    appid_connection_id.push_back(
        std::pair<uint32_t, uint32_t>(i, kCountOfRequestsForTest - i));
  }
  std::vector<std::pair<uint32_t, uint32_t>>::iterator req_it =
      appid_connection_id.begin();
  const std::vector<std::pair<uint32_t, uint32_t>>::iterator end =
      appid_connection_id.end();

  for (; req_it != end; ++req_it) {
    utils::SharedPtr<RequestInfoForTesting> request =
        CreateTestInfo(req_it->first,
                       req_it->second,
                       request_info::RequestInfo::HMIRequest,
                       kDefaultTimeout);
    EXPECT_TRUE(request_info_set_.Add(request));
  }

  request_info::RequestInfoPtr request =
      request_info_set_.Find(kCountOfRequestsForTest, kCountOfRequestsForTest);
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
  const uint32_t kConnectionKey = 65483u;
  const uint32_t kCorrId = 65483u;
  utils::SharedPtr<RequestInfoForTesting> request =
      CreateTestInfo(kConnectionKey,
                     kCorrId,
                     request_info::RequestInfo::HMIRequest,
                     kDefaultTimeout);
  EXPECT_TRUE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  EXPECT_EQ(1u, request_info_set.Size());
  request_info::RequestInfoPtr found =
      request_info_set.Find(kConnectionKey, kCorrId);
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
  TimevalStruct not_expired = date_time::DateTime::getCurrentTime();
  not_expired.tv_usec = std::numeric_limits<time_t>::min();

  TimevalStruct expired = date_time::DateTime::getCurrentTime();
  expired.tv_usec = std::numeric_limits<time_t>::max();

  utils::SharedPtr<RequestInfoForTesting> request =
      CreateTestInfo(kMobileConnectionKey1,
                     mobile_correlation_id,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);

  request->SetEndTime(expired);
  EXPECT_FALSE(request->isExpired());

  request->SetEndTime(not_expired);
  EXPECT_TRUE(request->isExpired());
}

TEST_F(RequestInfoTest, UpdateEndTime) {
  const TimevalStruct& end_time = date_time::DateTime::getCurrentTime();
  utils::SharedPtr<RequestInfoForTesting> request =
      CreateTestInfo(kMobileConnectionKey1,
                     mobile_correlation_id,
                     request_info::RequestInfo::MobileRequest,
                     kDefaultTimeout);
  request->SetEndTime(end_time);
  request->updateEndTime();
  const TimevalStruct& last_time = request->end_time();
  EXPECT_LE(end_time.tv_sec, last_time.tv_sec);
}

TEST_F(RequestInfoTest, EraseCheckStates) {
  for (uint32_t i = 0; i < kCountOfRequestsForTest; ++i) {
    utils::SharedPtr<RequestInfoForTesting> request = CreateTestInfo(
        kMobileConnectionKey1, i, request_info::RequestInfo::HMIRequest, i);
    request_info_set_.Add(request);
  }
  // Delete all RequestInfo
  request_info::RequestInfoPtr request_info;
  for (uint32_t i = 1; i < kCountOfRequestsForTest; ++i) {
    request_info = request_info_set_.Front();
    EXPECT_TRUE(request_info.valid());
    EXPECT_EQ(0u, request_info->timeout_msec());
    request_info = request_info_set_.FrontWithNotNullTimeout();
    EXPECT_TRUE(request_info.valid());
    EXPECT_EQ(i, request_info->timeout_msec());
    EXPECT_TRUE(request_info_set_.RemoveRequest(request_info));
  }
  // Delete RequestInfo from empty request_info_set_
  EXPECT_FALSE(request_info_set_.RemoveRequest(request_info));
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test
