#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "request_watchdog/watchdog_subscriber.h"
#include "request_watchdog/request_watchdog.h"

namespace test {
namespace components {
namespace request_watchdog_test
{

class RequestWatchdogTest : public ::testing::Test {
protected:
  virtual void SetUp() {

    for(int i = 0; i < 100000; i++) {
      if(i < 10) {
        smallRequestList.push_back(generateRequestInfo());
      }

      if(i < 100) {
        normalRequestList.push_back(generateRequestInfo());
      }

      hugeRequestList.push_back(generateRequestInfo());
    }
  }

  virtual void TearDown() {
    request_watchdog::RequestWatchdog::getRequestWatchdog()->removeAllListeners();
    request_watchdog::RequestWatchdog::getRequestWatchdog()->removeAllRequests();
  }

  class WatchdogInstantiator : public threads::IThreadDelegate {
   private:
    request_watchdog::RequestWatchdog instance_;
   public:
    void threadMain() {
      instance_ = request_watchdog::RequestWatchdog::getRequestWatchdog();
    }
  };

  std::list<request_watchdog::RequestInfo> smallRequestList;
  std::list<request_watchdog::RequestInfo> normalRequestList;
  std::list<request_watchdog::RequestInfo> hugeRequestList;

  request_watchdog::RequestInfo generateRequestInfo()
  {
    return request_watchdog::RequestInfo(rand() % INT_MAX, rand() % INT_MAX, rand() % INT_MAX, rand() % INT_MAX);
  }
};

class RequestWatchdogSubscriberTest : public request_watchdog::WatchdogSubscriber {
public:
  /**
  * Default constructor
  */
  RequestWatchdogSubscriberTest(): WatchdogSubscriber() {
  }

  virtual void onTimeoutExpired(request_watchdog::RequestInfo requestInfo)
  {

  }

  /**
  * Default destructor
  */
  virtual ~RequestWatchdogSubscriberTest() {
    destructor();
  }

  MOCK_METHOD0(destructor, void());
};

TEST_F(RequestWatchdogTest, Constructor) {
  test::components::request_watchdog_test::RequestWatchdogSubscriberTest* object1 =
    new test::components::request_watchdog_test::RequestWatchdogSubscriberTest();

  EXPECT_CALL(*object1, destructor()).Times(0);
  EXPECT_CALL(*object1, destructor()).Times(1);
  delete object1;
}

TEST_F(RequestWatchdogTest, GetWachdogInsanceSingleThread) {

  request_watchdog::Watchdog* watchdogInstance = request_watchdog::RequestWatchdog::getRequestWatchdog();

  ASSERT_TRUE(watchdogInstance != 0);
}

TEST_F(RequestWatchdogTest, SimpleAddRequest) {

  request_watchdog::Watchdog* watchdogInstance = request_watchdog::RequestWatchdog::getRequestWatchdog();

  ASSERT_TRUE(watchdogInstance != 0);

  watchdogInstance->addRequest(generateRequestInfo());

  ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());
}

TEST_F(RequestWatchdogTest, SimpleAddRemoveRequest) {

  request_watchdog::Watchdog* watchdogInstance = request_watchdog::RequestWatchdog::getRequestWatchdog();

  ASSERT_TRUE(watchdogInstance != 0);

  request_watchdog::RequestInfo requestOne = generateRequestInfo();
  request_watchdog::RequestInfo requestTwo = generateRequestInfo();

  if(requestOne == requestTwo)
  {
    watchdogInstance->addRequest(requestOne);
    ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->addRequest(requestOne);
    ASSERT_EQ(2, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->removeRequest(requestTwo);
    ASSERT_EQ(1, watchdogInstance->getRegesteredRequestsNumber());

    watchdogInstance->removeRequest(requestTwo);
    ASSERT_EQ(0, watchdogInstance->getRegesteredRequestsNumber());
  }
  else
  {
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

} // namespace request_watchdaog
} // namespace components
} // namespace test

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
