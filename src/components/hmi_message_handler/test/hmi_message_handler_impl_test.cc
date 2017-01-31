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
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "application_manager/message.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "hmi_message_handler/messagebroker_adapter.h"
#include "hmi_message_handler/mock_hmi_message_observer.h"
#include "hmi_message_handler/mock_hmi_message_handler_settings.h"
#include "hmi_message_handler/mock_hmi_message_adapter.h"

namespace test {
namespace components {
namespace hmi_message_handler_test {

using ::testing::Return;
using ::testing::_;
using namespace ::hmi_message_handler;

class HMIMessageHandlerImplTest : public ::testing::Test {
 public:
  HMIMessageHandlerImplTest()
      : hmi_handler_(InitHmiHandler())
      , mock_hmi_message_observer_(utils::MakeShared<MockHMIMessageObserver>())
      , message_adapter_(utils::MakeShared<MockHMIMessageAdapter>()) {
    hmi_handler_->set_message_observer(mock_hmi_message_observer_.get());
  }

  void TearDown() OVERRIDE {
    hmi_handler_.reset();
  }

 protected:
  testing::NiceMock<MockHMIMessageHandlerSettings>
      mock_hmi_message_handler_settings_;
  utils::SharedPtr<HMIMessageHandlerImpl> hmi_handler_;
  utils::SharedPtr<MockHMIMessageObserver> mock_hmi_message_observer_;
  utils::SharedPtr<MockHMIMessageAdapter> message_adapter_;
  static const uint64_t stack_size = 1000u;

  utils::SharedPtr<HMIMessageHandlerImpl> InitHmiHandler() {
    EXPECT_CALL(mock_hmi_message_handler_settings_, thread_min_stack_size())
        .WillRepeatedly(Return(stack_size));
    return utils::MakeShared<HMIMessageHandlerImpl>(
        mock_hmi_message_handler_settings_);
  }

  MessageSharedPointer CreateMessage() {
    // The ServiceType doesn't really matter
    return new application_manager::Message(
        protocol_handler::MessagePriority::FromServiceType(
            protocol_handler::ServiceType::kInvalidServiceType));
  }
};

TEST_F(HMIMessageHandlerImplTest,
       OnErrorSending_EmptyMessage_OnErrorSendingProceeded) {
  // Arrange
  MessageSharedPointer empty_message;
  EXPECT_CALL(*mock_hmi_message_observer_, OnErrorSending(empty_message));
  // Act
  hmi_handler_->OnErrorSending(empty_message);
}

TEST_F(HMIMessageHandlerImplTest,
       OnErrorSending_NotEmptyMessage_ExpectOnErrorSendingProceeded) {
  // Arrange
  utils::SharedPtr<application_manager::Message> message = CreateMessage();

  EXPECT_CALL(*mock_hmi_message_observer_, OnErrorSending(message));
  // Act
  hmi_handler_->OnErrorSending(message);
}

TEST_F(HMIMessageHandlerImplTest, OnErrorSending_InvalidObserver_Cancelled) {
  // Arrange
  utils::SharedPtr<application_manager::Message> message = CreateMessage();

  hmi_handler_->set_message_observer(NULL);
  EXPECT_CALL(*mock_hmi_message_observer_, OnErrorSending(_)).Times(0);
  // Act
  hmi_handler_->OnErrorSending(message);
}

TEST_F(HMIMessageHandlerImplTest,
       AddHMIMessageAdapter_AddExistedAdapter_ExpectAdded) {
  // Check before action
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
  // Act
  hmi_handler_->AddHMIMessageAdapter(message_adapter_.get());
  // Check after action
  EXPECT_EQ(1u, hmi_handler_->message_adapters().size());
}

TEST_F(HMIMessageHandlerImplTest,
       AddHMIMessageAdapter_AddUnexistedAdapter_ExpectNotAdded) {
  // Check before action
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
  // Act
  hmi_handler_->AddHMIMessageAdapter(NULL);
  // Check adapter not added
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
}

TEST_F(HMIMessageHandlerImplTest, RemoveHMIMessageAdapter_ExpectRemoved) {
  // Arrange
  hmi_handler_->AddHMIMessageAdapter(message_adapter_.get());
  // Act
  hmi_handler_->RemoveHMIMessageAdapter(message_adapter_.get());
  // Check after action
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
}

TEST_F(HMIMessageHandlerImplTest, OnMessageReceived_ValidObserver_Success) {
  MessageSharedPointer message = CreateMessage();
  EXPECT_CALL(*mock_hmi_message_observer_, OnMessageReceived(message));

  hmi_handler_->OnMessageReceived(message);
  // Wait for the message to be processed
  hmi_handler_->messages_from_hmi()->WaitDumpQueue();
}

TEST_F(HMIMessageHandlerImplTest, OnMessageReceived_InvalidObserver_Cancelled) {
  MessageSharedPointer message = CreateMessage();
  EXPECT_CALL(*mock_hmi_message_observer_, OnMessageReceived(_)).Times(0);
  // Make the observer invalid
  hmi_handler_->set_message_observer(NULL);
  hmi_handler_->OnMessageReceived(message);
  hmi_handler_->messages_from_hmi()->WaitDumpQueue();
}

TEST_F(HMIMessageHandlerImplTest, SendMessageToHMI_Success) {
  MessageSharedPointer message = CreateMessage();

  EXPECT_CALL(*message_adapter_, SendMessageToHMI(message));

  hmi_handler_->AddHMIMessageAdapter(message_adapter_.get());
  hmi_handler_->SendMessageToHMI(message);

  // Wait for the message to be processed
  hmi_handler_->messages_to_hmi()->WaitDumpQueue();
}

}  // namespace hmi_message_handler_test
}  // namespace components
}  // namespace test
