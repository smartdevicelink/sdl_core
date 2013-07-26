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

#ifndef TEST_COMPONENTS_MOBILE_MESSAGE_HANDLER_INCLUDE_MOBILE_MESSAGE_HANDLER_MOBILE_MESSAGE_HANDLER_TEST_H_
#define TEST_COMPONENTS_MOBILE_MESSAGE_HANDLER_INCLUDE_MOBILE_MESSAGE_HANDLER_MOBILE_MESSAGE_HANDLER_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "mobile_message_handler/mobile_message_handler_impl.h"
#include "protocol_handler/protocol_handler.h"
#include "utils/logger.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/session_observer.h"

#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

#include "utils/synchronisation_primitives.h"
#include "utils/timer.h"

//! ---------------------------------------------------------------------------

log4cxx::LoggerPtr logger = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("mobile_message_handler_test"));

sync_primitives::SynchronisationPrimitives synchronisation;
sync_primitives::Timer* timer;
const unsigned int kTimeout = 2;
bool flag = false;

//! ---------------------------------------------------------------------------
namespace test {
namespace components {
namespace mobile_message_handler_test {
/**
 * @class MobileMessageHandlerTester implements ApplicationManager
 * and ProtocolHandler logic.
 */
class MobileMessageHandlerTester :
    public mobile_message_handler::MobileMessageObserver,
    public protocol_handler::ProtocolHandler {
 public:
  virtual void set_protocol_observer(protocol_handler::ProtocolObserver* observer) {}

  /**
   * \brief Sets pointer for Connection Handler layer for managing sessions
   * \param observer Pointer to object of the class implementing
   * ISessionObserver
   */
  virtual void set_session_observer(protocol_handler::SessionObserver* observer) {}

  /**
   * \brief Method for sending message to Mobile Application.
   * \param message RawMessage with params to be sent to Mobile App.
   */
  void SendMessageToMobileApp(const transport_manager::RawMessageSptr& message) {}

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

  void sendMessageToMobileApp(const transport_manager::RawMessageSptr message) {
    mmh_->OnMessageReceived(message);
  }

 private:
  mobile_message_handler::MobileMessageHandlerImpl* mmh_;
  MobileMessage message_;

  DISALLOW_COPY_AND_ASSIGN(MobileMessageHandlerTester);
};

/**
 * @class MobileMessageHandlerTestObserverThread
 */
class MobileMessageHandlerTestObserverThread : public threads::ThreadDelegate {
 public:
  explicit MobileMessageHandlerTestObserverThread(const MobileMessage& message)
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
}  // namespace mobile_message_handler_test
}  // namespace components
}  // namespace test

#endif  // TEST_COMPONENTS_MOBILE_MESSAGE_HANDLER_INCLUDE_MOBILE_MESSAGE_HANDLER_MOBILE_MESSAGE_HANDLER_TEST_H_
