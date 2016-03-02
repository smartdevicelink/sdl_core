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

#include "gtest/gtest.h"
#include "application_manager/message.h"
#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "hmi_message_handler/messagebroker_adapter.h"
#include "hmi_message_handler/mock_hmi_message_observer.h"
#include "hmi_message_handler/mock_hmi_message_handler_settings.h"
#include "utils/make_shared.h"

namespace test {
namespace components {
namespace hmi_message_handler_test {

using ::testing::ReturnRef;
class HMIMessageHandlerImplTest : public ::testing::Test {
 public:
  HMIMessageHandlerImplTest()
      : mb_adapter_(NULL),
        hmi_handler_(NULL),
        mock_hmi_message_observer_(NULL) {}

 protected:
  hmi_message_handler::MessageBrokerAdapter* mb_adapter_;
  hmi_message_handler::HMIMessageHandlerImpl* hmi_handler_;
  hmi_message_handler::MockHMIMessageObserver* mock_hmi_message_observer_;
  testing::NiceMock<MockHMIMessageHandlerSettings> mock_hmi_message_handler_settings;
  const uint64_t stack_size =1000u;

  virtual void SetUp() OVERRIDE {
    ON_CALL(mock_hmi_message_handler_settings, thread_min_stack_size())
        .WillByDefault(ReturnRef(stack_size));
    hmi_handler_ = new hmi_message_handler::HMIMessageHandlerImpl(
        mock_hmi_message_handler_settings);
    mb_adapter_ = new hmi_message_handler::MessageBrokerAdapter(
        hmi_handler_, "localhost", 22);
    mock_hmi_message_observer_ =
        new hmi_message_handler::MockHMIMessageObserver();
    hmi_handler_->set_message_observer(mock_hmi_message_observer_);
    EXPECT_TRUE(NULL != hmi_handler_->observer());
  }

  virtual void TearDown() OVERRIDE {
    hmi_handler_->set_message_observer(NULL);
    delete mock_hmi_message_observer_;
    delete hmi_handler_;
    delete mb_adapter_;
  }
};

TEST_F(HMIMessageHandlerImplTest,
       OnErrorSending_EmptyMessage_OnErrorSendingProceeded) {
  // Arrange
  hmi_message_handler::MessageSharedPointer empty_message;
  EXPECT_CALL(*mock_hmi_message_observer_, OnErrorSending(empty_message));
  // Act
  hmi_handler_->OnErrorSending(empty_message);
}

TEST_F(HMIMessageHandlerImplTest,
       OnErrorSending_NotEmptyMessage_ExpectOnErrorSendingProceeded) {
  // Arrange
  utils::SharedPtr<application_manager::Message> message(
      utils::MakeShared<application_manager::Message>(
          protocol_handler::MessagePriority::FromServiceType(
              protocol_handler::ServiceType::kControl)));

  EXPECT_CALL(*mock_hmi_message_observer_, OnErrorSending(message));
  // Act
  hmi_handler_->OnErrorSending(message);
}

TEST_F(HMIMessageHandlerImplTest,
       AddHMIMessageAdapter_AddExistedAdapter_ExpectAdded) {
  // Check before action
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
  // Act
  hmi_handler_->AddHMIMessageAdapter(mb_adapter_);
  // Check after action
  EXPECT_EQ(1u, hmi_handler_->message_adapters().size());
}

TEST_F(HMIMessageHandlerImplTest,
       AddHMIMessageAdapter_AddUnexistedAdapter_ExpectNotAdded) {
  // Check before action
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
  // Act
  mb_adapter_ = NULL;
  hmi_handler_->AddHMIMessageAdapter(mb_adapter_);
  // Check adapter not added
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
}

TEST_F(HMIMessageHandlerImplTest, RemoveHMIMessageAdapter_ExpectRemoved) {
  // Arrange
  hmi_handler_->AddHMIMessageAdapter(mb_adapter_);
  // Act
  hmi_handler_->RemoveHMIMessageAdapter(mb_adapter_);
  // Check after action
  EXPECT_TRUE(hmi_handler_->message_adapters().empty());
}

}  // namespace hmi_message_handler_test
}  // namespace components
}  // namespace test
