/**
* \file request_watchdog_test.cc
* \brief RequetWatchdog test source file.
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

#include <list>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "request_watchdog/watchdog_subscriber.h"
#include "request_watchdog/request_watchdog.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace test {
namespace components {
namespace request_watchdog_test {

class RequestWatchdogTest : public ::testing::Test {
 protected:
    virtual void SetUp() {
    for ( int i = 0; i < 100000; i++ ) {
      if ( i < 10 ) {
        smallRequestList.push_back(generateRequestInfo());
      }

      if ( i < 100 ) {
        normalRequestList.push_back(generateRequestInfo());
      }

      hugeRequestList.push_back(generateRequestInfo());
    }
  }

  virtual void TearDown() {
    request_watchdog::RequestWatchdog::
      getRequestWatchdog()->removeAllListeners();

    request_watchdog::RequestWatchdog::
      getRequestWatchdog()->removeAllRequests();
  }

  class WatchdogInstantiator : public threads::ThreadDelegate {
   private:
    request_watchdog::Watchdog* instance_;
   public:
    void threadMain() {
      instance_ = request_watchdog::RequestWatchdog::getRequestWatchdog();
    }
  };

  std::list<request_watchdog::RequestInfo> smallRequestList;
  std::list<request_watchdog::RequestInfo> normalRequestList;
  std::list<request_watchdog::RequestInfo> hugeRequestList;
  unsigned int seed;

  request_watchdog::RequestInfo generateRequestInfo() {
    return request_watchdog::RequestInfo(rand_r(&seed) % INT_MAX,
                                         rand_r(&seed) % INT_MAX,
                                         rand_r(&seed) % INT_MAX,
                                         rand_r(&seed) % INT_MAX);
  }
};

class RequestWatchdogSubscriberTest : public request_watchdog::
  WatchdogSubscriber {
 public:
  /**
  * Default constructor
  */
  RequestWatchdogSubscriberTest(): WatchdogSubscriber() {
  }

  virtual void onTimeoutExpired(request_watchdog::RequestInfo requestInfo) {
    timeoutExpiredCallback(requestInfo);
  }

  /**
  * Default destructor
  */
  virtual ~RequestWatchdogSubscriberTest() {
    destructor();
  }

  MOCK_METHOD0(destructor, void());
  MOCK_METHOD1(timeoutExpiredCallback, void(request_watchdog::RequestInfo requestInfo));
};

TEST_F(RequestWatchdogTest, Constructor) {
  test::components::request_watchdog_test::
    RequestWatchdogSubscriberTest* object1 =
  new test::components::request_watchdog_test::
    RequestWatchdogSubscriberTest();

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object1, destructor()).Times(1);
  delete object1;
}

TEST_F(RequestWatchdogTest, GetWachdogInsanceSingleThread) {
  request_watchdog::Watchdog* watchdogInstance =
      request_watchdog::RequestWatchdog::getRequestWatchdog();

  ASSERT_TRUE(watchdogInstance != 0);
}

TEST_F(RequestWatchdogTest, SimpleAddRequest) {
  request_watchdog::Watchdog* watchdogInstance =
      request_watchdog::RequestWatchdog::getRequestWatchdog();

  ASSERT_TRUE(watchdogInstance != 0);

  watchdogInstance->addRequest(generateRequestInfo());

  ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());
}

TEST_F(RequestWatchdogTest, SimpleAddRemoveRequest) {
  request_watchdog::Watchdog* watchdogInstance =
      request_watchdog::RequestWatchdog::getRequestWatchdog();

  ASSERT_TRUE(watchdogInstance != 0);

  request_watchdog::RequestInfo requestOne = generateRequestInfo();
  request_watchdog::RequestInfo requestTwo = generateRequestInfo();

  if ( requestOne == requestTwo ) {
    watchdogInstance->addRequest(requestOne);
    ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->addRequest(requestOne);
    ASSERT_EQ(2, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->removeRequest(requestTwo);
    ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->removeRequest(requestTwo);
    ASSERT_EQ(0, watchdogInstance->getRegesteredRequestsNumber());
  } else {
    watchdogInstance->addRequest(requestOne);
    ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->addRequest(requestOne);
    ASSERT_EQ(2, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->addRequest(requestTwo);
    ASSERT_EQ(3, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->removeRequest(requestTwo);
    ASSERT_EQ(2, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->removeRequest(requestTwo);
    ASSERT_EQ(2, watchdogInstance->getRegesteredRequestsNumber());
  }
}

}  //  namespace request_watchdog_test
}  //  namespace components
}  //  namespace test

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
