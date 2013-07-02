#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "utils/logger.h"

#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

#include "utils/synchronisation_primitives.h"
#include "utils/timer.h"

//! ---------------------------------------------------------------------------

namespace {
log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("mobile_message_handler_test"));

sync_primitives::SynchronisationPrimitives synchronisation;
sync_primitives::Timer* timer;
const unsigned int kTimeout = 2;
bool flag = false;
}

//! ---------------------------------------------------------------------------

namespace mobile_message_handler_test {
/**
 * @class MobileMessageHandlerTester implements ApplicationManager
 * and ProtocolHandler logic.
 */
class MobileMessageHandlerTester :
    public mobile_message_handler::MobileMessageObserver,
    public protocol_handler::ProtocolHandler {
 public:
  MobileMessageHandlerTester()
      : mmh_(NULL) {
  }

  bool init(const MobileMessage& message) {
    message_ = message;
    mmh_ = mobile_message_handler::MobileMessageHandlerImpl::instance();
    DCHECK(mmh_);

    return true;
  }

  void OnMobileMessageReceived(const MobileMessage& message) {
    ASSERT_TRUE(message_->operator ==(*message));

    flag = true;
    synchronisation.signal();
  }

  void sendMessageToMobileApp(const protocol_handler::RawMessage* message) {
    mmh_->onMessageReceived(message);
  }

 private:
  mobile_message_handler::MobileMessageHandlerImpl* mmh_;
  MobileMessage message_;DISALLOW_COPY_AND_ASSIGN(MobileMessageHandlerTester);
};

/**
 * @class MobileMessageHandlerTestObserverThread
 */
class MobileMessageHandlerTestObserverThread : public threads::ThreadDelegate {
 public:
  MobileMessageHandlerTestObserverThread(const MobileMessage& message)
      : message_(message) {
  }
  ~MobileMessageHandlerTestObserverThread() {
  }

  void threadMain() {

    mobile_message_handler::MobileMessageHandlerImpl* mmh =
        mobile_message_handler::MobileMessageHandlerImpl::instance();
    DCHECK(mmh);

    mmh->SendMessageToMobileApp(message_);
    timer->StartWait(kTimeout);
    ASSERT_TRUE(flag);
  }

 private:
  MobileMessage message_;
  DISALLOW_COPY_AND_ASSIGN(MobileMessageHandlerTestObserverThread);
};

//! ---------------------------------------------------------------------------

TEST(mobile_message_handler_test, component_test) {
  // Synchronization primitives initialization.
  synchronisation.init();
  timer = new sync_primitives::Timer(&synchronisation);
  DCHECK(timer);

  // Example message
  MobileMessage message(new application_manager::Message);
  application_manager::BinaryData binary_data;
  binary_data.push_back('X');
  message->set_binary_data(&binary_data);
  message->set_connection_key(100);
  message->set_correlation_id(10);
  message->set_function_id(5);
  message->set_json_message("test json string!!!");
  message->set_message_type(application_manager::kRequest);
  message->set_protocol_version(application_manager::kV2);

  // Component initialization.
  MobileMessageHandlerTester observer;
  observer.init(message);

  mobile_message_handler::MobileMessageHandlerImpl* mmh =
      mobile_message_handler::MobileMessageHandlerImpl::instance();
  DCHECK(mmh);
  mmh->setProtocolHandler(&observer);
  mmh->AddMobileMessageListener(&observer);

  // Message processing
  threads::Thread* observer_thread = new threads::Thread(
      "MobileMessageHandler::MobileMessageHandlerTestObserverThread",
      new MobileMessageHandlerTestObserverThread(message));

  observer_thread->start();
  observer_thread->join();
}

//! ---------------------------------------------------------------------------

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  log4cxx::PropertyConfigurator::configure("log4cxx.properties");

  return RUN_ALL_TESTS();
}}
