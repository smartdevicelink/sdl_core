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

#ifndef TEST_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_WATCHDOG_TEST_H_
#define TEST_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_WATCHDOG_TEST_H_

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

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("request_watchdog_test"));

class RequestWatchdogTest : public ::testing::Test {
  protected:
    virtual void SetUp();
    virtual void TearDown();

    class WatchdogInstantiator : public threads::ThreadDelegate {
      private:
        request_watchdog::Watchdog* instance_;
      public:
        void threadMain() {
          instance_ = request_watchdog::RequestWatchdog::instance();
        }
    };

    std::list<request_watchdog::RequestInfo> smallRequestList;
    std::list<request_watchdog::RequestInfo> normalRequestList;
    std::list<request_watchdog::RequestInfo> hugeRequestList;
    unsigned int seed;

    request_watchdog::RequestInfo generateRequestInfo();
};

class RequestWatchdogSubscriber : public request_watchdog::
    WatchdogSubscriber {
  public:
    /**
    * Default constructor
    */
    RequestWatchdogSubscriber();

    virtual void onTimeoutExpired(request_watchdog::RequestInfo requestInfo);

    /**
    * Default destructor
    */
    virtual ~RequestWatchdogSubscriber();

    MOCK_METHOD0(destructor, void());
};

void RequestWatchdogTest::SetUp() {
  for (int i = 0; i < 100000; i++) {
    if (i < 10) {
      smallRequestList.push_back(generateRequestInfo());
    }

    if (i < 100) {
      normalRequestList.push_back(generateRequestInfo());
    }

    hugeRequestList.push_back(generateRequestInfo());
  }
}

void RequestWatchdogTest::TearDown() {
  request_watchdog::RequestWatchdog::
  instance()->removeAllListeners();

  request_watchdog::RequestWatchdog::
  instance()->removeAllRequests();
}

request_watchdog::RequestInfo RequestWatchdogTest::generateRequestInfo() {
  return request_watchdog::RequestInfo(rand_r(&seed) % INT_MAX,
                                       rand_r(&seed) % INT_MAX,
                                       rand_r(&seed) % INT_MAX,
                                       rand_r(&seed) % INT_MAX);
}

RequestWatchdogSubscriber::RequestWatchdogSubscriber():
  WatchdogSubscriber() {
}

RequestWatchdogSubscriber::~RequestWatchdogSubscriber() {
  destructor();
}

void RequestWatchdogSubscriber::onTimeoutExpired(request_watchdog::RequestInfo requestInfo) {
  LOG4CXX_TRACE_ENTER(logger);
  LOG4CXX_INFO(logger, this << "::onTimeoutExpired\n"
               << "for the following  request: "
               << "\n ConnectionID : " << requestInfo.connectionID_
               << "\n CorrelationID : " << requestInfo.correlationID_
               << "\n FunctionID : " << requestInfo.functionID_
               << "\n CustomTimeOut : " << requestInfo.customTimeout_
               << "\n");
}

TEST_F(RequestWatchdogTest, Constructor) {
  test::components::request_watchdog_test::
  RequestWatchdogSubscriber* object1 =
    new test::components::request_watchdog_test::
  RequestWatchdogSubscriber();

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object1, destructor()).Times(1);
  delete object1;
}

/*TEST_F(RequestWatchdogTest, TimeoutExpiredCallbackTest) {
  request_watchdog::WatchdogSubscriber* subscriber_one =
      new RequestWatchdogSubscriber();
  request_watchdog::WatchdogSubscriber* subscriber_two =
      new RequestWatchdogSubscriber();

  request_watchdog::RequestWatchdog::
    instance()->addListener(subscriber_one);

  request_watchdog::RequestWatchdog::
      instance()->addListener(subscriber_two);

  usleep(10000000);

  request_watchdog::RequestInfo requestInfo = request_watchdog::RequestInfo(
                                rand_r(&seed) % INT_MAX,
                                rand_r(&seed) % INT_MAX,
                                rand_r(&seed) % INT_MAX,
                                5000);

  request_watchdog::RequestWatchdog::
      instance()->addRequest(requestInfo);

  requestInfo = request_watchdog::RequestInfo(
                                  rand_r(&seed) % INT_MAX,
                                  rand_r(&seed) % INT_MAX,
                                  rand_r(&seed) % INT_MAX,
                                  3000);

  request_watchdog::RequestWatchdog::
        instance()->addRequest(requestInfo);

  request_watchdog::RequestWatchdog::
        instance()->addRequest(generateRequestInfo());

  usleep(10000000);

  ASSERT_EQ(1, request_watchdog::RequestWatchdog::
          instance()->getRegesteredRequestsNumber());

  request_watchdog::RequestWatchdog::
        instance()->removeAllRequests();

  usleep(10000000);

  ASSERT_EQ(0, request_watchdog::RequestWatchdog::
        instance()->getRegesteredRequestsNumber());

  usleep(10000000);

  request_watchdog::RequestWatchdog::
        instance()->addRequest(generateRequestInfo());

  usleep(10000000);

  request_watchdog::RequestWatchdog::
        instance()->removeAllListeners();

  delete subscriber_one;
  delete subscriber_two;
}*/

TEST_F(RequestWatchdogTest, GetWachdogInsanceSingleThread) {
  request_watchdog::Watchdog* watchdogInstance =
    request_watchdog::RequestWatchdog::instance();

  ASSERT_TRUE(watchdogInstance != 0);
}

TEST_F(RequestWatchdogTest, SimpleAddRequest) {
  request_watchdog::Watchdog* watchdogInstance =
    request_watchdog::RequestWatchdog::instance();

  ASSERT_TRUE(watchdogInstance != 0);

  watchdogInstance->addRequest(generateRequestInfo());

  ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());
}

TEST_F(RequestWatchdogTest, SimpleAddRemoveRequest) {
  request_watchdog::Watchdog* watchdogInstance =
    request_watchdog::RequestWatchdog::instance();

  ASSERT_TRUE(watchdogInstance != 0);

  request_watchdog::RequestInfo requestOne = generateRequestInfo();
  request_watchdog::RequestInfo requestTwo = generateRequestInfo();

  watchdogInstance->addRequest(requestOne);
  ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

  watchdogInstance->addRequest(requestOne);
  ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

  watchdogInstance->addRequest(requestTwo);
  ASSERT_EQ(2, watchdogInstance->getRegesteredRequestsNumber());

  watchdogInstance->removeRequest(
    requestTwo.connectionID_, requestTwo.correlationID_);
  ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

  watchdogInstance->removeRequest(
    requestTwo.connectionID_, requestTwo.correlationID_);
  ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

  watchdogInstance->removeRequest(
    requestOne.connectionID_, requestOne.correlationID_);
  ASSERT_EQ(0, watchdogInstance->getRegesteredRequestsNumber());
}

}  //  namespace request_watchdog_test
}  //  namespace components
}  //  namespace test

#endif  // TEST_COMPONENTS_REQUEST_WATCHDOG_INCLUDE_REQUEST_WATCHDOG_REQUEST_WATCHDOG_TEST_H_

