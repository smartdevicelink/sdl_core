#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "application_manager/request_info.h"
#include "application_manager/message_helper.h"
#include <iostream>

namespace request_info = application_manager::request_controller;

class  MockRequest: public application_manager::commands::Command {
  public:
    MockRequest(uint32_t connection_key,
                uint32_t correlation_id):
      connection_key_(connection_key),
      correlation_id_(correlation_id) {

    }
    MOCK_METHOD0(CheckPermissions, bool ());
    MOCK_METHOD0(Init, bool ());
    MOCK_METHOD0(Run, void ());
    MOCK_METHOD0(CleanUp, bool ());
    MOCK_CONST_METHOD0(default_timeout, uint32_t ());
    MOCK_CONST_METHOD0(function_id, int32_t ());
    MOCK_METHOD0(onTimeOut, void ());
    MOCK_METHOD0(AllowedToTerminate, bool ());
    MOCK_METHOD1(SetAllowedToTerminate, void (bool));

    uint32_t connection_key_;
    uint32_t correlation_id_;
    virtual uint32_t connection_key() const;
    virtual uint32_t correlation_id() const;
};

class  TestRequestInfo: public request_info::RequestInfo {
  public:
    TestRequestInfo(request_info::RequestPtr request,
                    const RequestType requst_type,
                    const TimevalStruct& start_time,
                    const  uint64_t timeout_sec):
      RequestInfo(request, requst_type, start_time,timeout_sec) {

    }
    void SetEndTime(const TimevalStruct& end_time);
};
class RequestInfoTest : public ::testing::Test {
  protected:
    virtual void SetUp() {
      //INIT_LOGGER("log4cxx.properties");
      count_of_requests_for_test_ = 1000;
      hmi_connection_key_ = 0;
      mobile_connection_key1_ = 65431;
      mobile_connection_key2_ = 65123;
      default_timeout_ = 10;
      srand(42);
    }
    virtual void TearDown() {
    //DEINIT_LOGGER();
    }

    request_info::RequestInfoSet request_info_set_;
    uint32_t count_of_requests_for_test_ ;
    uint32_t hmi_connection_key_;
    uint32_t mobile_connection_key1_;
    uint32_t mobile_connection_key2_;
    uint32_t default_timeout_;

    utils::SharedPtr<TestRequestInfo> create_test_info(uint32_t connection_key,
                                                       uint32_t correlation_id,
                                                       request_info::RequestInfo::RequestType requst_type,
                                                       const TimevalStruct& start_time,
                                                       uint64_t timeout_sec) {
      utils::SharedPtr<MockRequest> mock_request(new MockRequest(connection_key,correlation_id));
      TestRequestInfo* test_request_raw = new TestRequestInfo(mock_request,requst_type,
                                    start_time, timeout_sec);
      utils::SharedPtr<TestRequestInfo> request(test_request_raw);
      return request;
    }
};

TEST_F(RequestInfoTest, RequestInfoEqualEndTimeTest) {
  std::list<utils::SharedPtr<TestRequestInfo> > requests;
  const TimevalStruct& time = date_time::DateTime::getCurrentTime();
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    utils::SharedPtr<TestRequestInfo> request =
        create_test_info(rand(), rand(), request_info::RequestInfo::MobileRequest,
                         time, default_timeout_);
    request->SetEndTime(time);
    EXPECT_TRUE(request_info_set_.Add(request));
  }
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetInsertErazeTest) {
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    utils::SharedPtr<TestRequestInfo> request =
        create_test_info(hmi_connection_key_, i, request_info::RequestInfo::HMIRequest,
                         date_time::DateTime::getCurrentTime(), default_timeout_);
      EXPECT_TRUE(request_info_set_.Add(request));
      EXPECT_EQ(1, request_info_set_.RemoveRequest(request));
  }

  EXPECT_EQ(0u, request_info_set_.Size());
  std::list<utils::SharedPtr<TestRequestInfo> > requests;

  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
      utils::SharedPtr<TestRequestInfo> request =
          create_test_info(hmi_connection_key_, i, request_info::RequestInfo::HMIRequest,
                           date_time::DateTime::getCurrentTime(), default_timeout_);
      requests.push_back(request);
      EXPECT_TRUE(request_info_set_.Add(request));
  }

  std::list<utils::SharedPtr<TestRequestInfo> >::iterator it = requests.begin();
  std::list<utils::SharedPtr<TestRequestInfo> >::iterator end = requests.end();

  for (; it != end; ++it) {
      EXPECT_EQ(1,request_info_set_.RemoveRequest(*it));
  }
  EXPECT_EQ(0u, request_info_set_.Size());
  it = requests.begin();
  for (; it != end; ++it) {
      EXPECT_TRUE(request_info_set_.Add(*it));
  }
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.Size());
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.RemoveByConnectionKey(hmi_connection_key_));
  EXPECT_EQ(0u, request_info_set_.Size());
  it = requests.begin();
  for (; it != end; ++it) {
      EXPECT_TRUE(request_info_set_.Add(*it));
  }
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.Size());
  utils::SharedPtr<TestRequestInfo> mobile_request1 =
      create_test_info(mobile_connection_key1_, 12345, request_info::RequestInfo::MobileRequest,
                       date_time::DateTime::getCurrentTime(), default_timeout_);
  EXPECT_TRUE(request_info_set_.Add(mobile_request1));
  utils::SharedPtr<TestRequestInfo> mobile_request2 =
      create_test_info(mobile_connection_key2_, 54321, request_info::RequestInfo::MobileRequest,
                       date_time::DateTime::getCurrentTime(), default_timeout_);
  EXPECT_TRUE(request_info_set_.Add(mobile_request2));
  EXPECT_EQ(count_of_requests_for_test_ + 2, request_info_set_.Size());
  EXPECT_EQ(2u, request_info_set_.RemoveMobileRequests());
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.Size());
  EXPECT_TRUE(request_info_set_.Add(mobile_request1));
  EXPECT_TRUE(request_info_set_.Add(mobile_request2));
  EXPECT_EQ(1u, request_info_set_.RemoveByConnectionKey(mobile_connection_key1_));
  EXPECT_EQ(count_of_requests_for_test_ + 1, request_info_set_.Size());
  EXPECT_EQ(count_of_requests_for_test_, request_info_set_.RemoveByConnectionKey(hmi_connection_key_));
  EXPECT_EQ(1u, request_info_set_.Size());
  EXPECT_EQ(1u, request_info_set_.RemoveMobileRequests());
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetFrontTest) {

  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
      utils::SharedPtr<TestRequestInfo> request =
          create_test_info(mobile_connection_key1_, i, request_info::RequestInfo::HMIRequest,
                           date_time::DateTime::getCurrentTime(), i);
      request_info_set_.Add(request);
  }

  for (uint32_t i = 1; i < count_of_requests_for_test_; ++i) {
    request_info::RequestInfoPtr request_info = request_info_set_.Front();
    EXPECT_TRUE(request_info.valid());
    EXPECT_EQ(0u, request_info->timeout_sec());
    request_info = request_info_set_.FrontWithNotNullTimeout();
    EXPECT_TRUE(request_info.valid());
    EXPECT_EQ(i, request_info->timeout_sec());
    EXPECT_TRUE(request_info_set_.RemoveRequest(request_info));
  }
  EXPECT_EQ(1u, request_info_set_.Size());
  EXPECT_EQ(1u, request_info_set_.RemoveByConnectionKey(mobile_connection_key1_));
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetFindTest) {

  std::list <std::pair<uint32_t, uint32_t> > appId_coorrId;
  for (uint32_t i = 0; i < count_of_requests_for_test_; ++i) {
    appId_coorrId.push_back(std::pair<uint32_t, uint32_t>(rand(),rand()));
  }
  std::list <std::pair<uint32_t, uint32_t> >::iterator it;
  std::list <std::pair<uint32_t, uint32_t> >::iterator end =
      appId_coorrId.end();;

  for (it = appId_coorrId.begin(); it != end; ++it) {
    utils::SharedPtr<TestRequestInfo> request =
        create_test_info(it->first, it->second, request_info::RequestInfo::HMIRequest,
                         date_time::DateTime::getCurrentTime(), 10);
    EXPECT_TRUE(request_info_set_.Add(request));
  }

  request_info::RequestInfoPtr request = request_info_set_.Find(rand(),rand());
  EXPECT_FALSE(request.valid());
  for (it = appId_coorrId.begin(); it != end; ++it) {
    request_info::RequestInfoPtr request = request_info_set_.Find(it->first, it->second);
    EXPECT_TRUE(request.valid());
    EXPECT_EQ(1u, request_info_set_.RemoveRequest(request));
    request = request_info_set_.Find(it->first, it->second);
    EXPECT_FALSE(request.valid());
  }
  EXPECT_EQ(0u, request_info_set_.Size());
}

TEST_F(RequestInfoTest, RequestInfoSetEqualHashTest) {
  request_info::RequestInfoSet request_info_set;
  const uint32_t connection_key = 65483;
  const uint32_t corr_id = 65483;
  utils::SharedPtr<TestRequestInfo> request =
      create_test_info(connection_key, corr_id, request_info::RequestInfo::HMIRequest,
                       date_time::DateTime::getCurrentTime(), 10);
  EXPECT_TRUE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  EXPECT_EQ(1u, request_info_set.Size());
  request_info::RequestInfoPtr found = request_info_set.Find(connection_key, corr_id);
  EXPECT_TRUE(found.valid());
  EXPECT_EQ(1u, request_info_set.RemoveRequest(found));
  EXPECT_EQ(0u, request_info_set.Size());
  EXPECT_TRUE(request_info_set.Add(request));
  EXPECT_FALSE(request_info_set.Add(request));
  found =  request_info_set.FrontWithNotNullTimeout();
  EXPECT_TRUE(found.valid());
  EXPECT_EQ(1u, request_info_set.RemoveRequest(found));
  found =  request_info_set.FrontWithNotNullTimeout();
  EXPECT_FALSE(found.valid());
  EXPECT_EQ(0u, request_info_set.Size());
}


uint32_t MockRequest::correlation_id() const {
  return correlation_id_;
}

uint32_t MockRequest::connection_key() const {
  return connection_key_;
}



void TestRequestInfo::SetEndTime(const TimevalStruct& end_time) {
    end_time_ = end_time;
}
