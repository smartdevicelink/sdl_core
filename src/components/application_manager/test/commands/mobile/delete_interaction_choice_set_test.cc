/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <stdint.h>
#include <map>

#include "mobile/delete_interaction_choice_set_request.h"
#include "mobile/delete_interaction_choice_set_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace delete_interaction_choice_set {

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

namespace am = ::application_manager;

using am::commands::DeleteInteractionChoiceSetRequest;
using am::commands::DeleteInteractionChoiceSetResponse;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;

typedef SharedPtr<DeleteInteractionChoiceSetRequest>
    DeleteInteractionChoiceSetRequestPtr;
typedef SharedPtr<DeleteInteractionChoiceSetResponse>
    DeleteInteractionChoiceSetResponsePtr;

MATCHER_P(CheckMessageSuccess, success, "") {
  return success ==
         (*arg)[am::strings::msg_params][am::strings::success].asBool();
}

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kChoiceSetId = 11u;
const uint32_t kChoiceId = 110u;
const uint32_t kGrammarId = 101u;
}  // namespace

class DeleteInteractionChoiceSetRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  DeleteInteractionChoiceSetRequestTest()
      : accessor_(choice_set_map_, performinteraction_choice_set_lock_) {}

  ~DeleteInteractionChoiceSetRequestTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(&app_mngr_);
  }

  am::PerformChoiceSetMap choice_set_map_;
  mutable sync_primitives::Lock performinteraction_choice_set_lock_;
  DataAccessor<am::PerformChoiceSetMap> accessor_;

 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    command_ = CreateCommand<DeleteInteractionChoiceSetRequest>(message_);
    app_ = CreateMockApp();
  }

  DeleteInteractionChoiceSetRequestPtr command_;
  MessageSharedPtr message_;
  MockAppPtr app_;
};

class DeleteInteractionChoiceSetResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    command_ = CreateCommand<DeleteInteractionChoiceSetResponse>(message_);
    app_ = CreateMockApp();
  }
  DeleteInteractionChoiceSetResponsePtr command_;
  MessageSharedPtr message_;
  MockAppPtr app_;
};

TEST_F(DeleteInteractionChoiceSetRequestTest, Run_InvalidApp_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL));
  EXPECT_CALL(*app_, FindChoiceSet(_)).Times(0);
  command_->Run();
}

TEST_F(DeleteInteractionChoiceSetRequestTest, Run_FindChoiceSetFail_UNSUCCESS) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL));

  command_->Run();
}

TEST_F(DeleteInteractionChoiceSetRequestTest, Run_ChoiceSetInUse_SUCCESS) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));

  smart_objects::SmartObject* choice_set_id =
      &((*message_)[am::strings::msg_params]
                   [am::strings::interaction_choice_set_id]);

  choice_set_map_[0].insert(
      std::make_pair(kChoiceSetId,
                     &((*message_)[am::strings::msg_params]
                                  [am::strings::interaction_choice_set_id])));

  EXPECT_CALL(*app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));
  EXPECT_CALL(*app_, is_perform_interaction_active()).WillOnce(Return(true));
  EXPECT_CALL(*app_, performinteraction_choice_set_map())
      .WillOnce(Return(accessor_));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL));

  command_->Run();
}

TEST_F(DeleteInteractionChoiceSetRequestTest,
       Run_SendVrDeleteCommand_PerformInteractionFalse_UNSUCCESS) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;
  smart_objects::SmartObject* choice_set_id =
      &((*message_)[am::strings::msg_params]
                   [am::strings::interaction_choice_set_id]);
  smart_objects::SmartObject* invalid_choice_set_id = NULL;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(app_));

  InSequence seq;

  EXPECT_CALL(*app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));
  EXPECT_CALL(*app_, is_perform_interaction_active()).WillOnce(Return(false));
  EXPECT_CALL(*app_, performinteraction_choice_set_map()).Times(0);

  EXPECT_CALL(*app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(invalid_choice_set_id));

  EXPECT_CALL(*app_, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(*app_, RemoveChoiceSet(kChoiceSetId));
  EXPECT_CALL(*app_, UpdateHash());

  DeleteInteractionChoiceSetRequestPtr command =
      CreateCommand<DeleteInteractionChoiceSetRequest>(message_);

  command->Init();
  command->Run();
}

TEST_F(DeleteInteractionChoiceSetRequestTest, Run_SendVrDeleteCommand_SUCCESS) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;
  (*message_)[am::strings::msg_params][am::strings::grammar_id] = kGrammarId;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::choice_id] = kChoiceId;
  smart_objects::SmartObject* choice_set_id =
      &((*message_)[am::strings::msg_params]);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(app_));

  InSequence seq;

  EXPECT_CALL(*app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));
  EXPECT_CALL(*app_, is_perform_interaction_active()).WillOnce(Return(false));
  EXPECT_CALL(*app_, performinteraction_choice_set_map()).Times(0);

  EXPECT_CALL(*app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(*app_, app_id())
      .WillOnce(Return(kConnectionKey))
      .WillOnce(Return(kConnectionKey));
  EXPECT_CALL(*app_, RemoveChoiceSet(kChoiceSetId));
  EXPECT_CALL(*app_, UpdateHash());

  DeleteInteractionChoiceSetRequestPtr command =
      CreateCommand<DeleteInteractionChoiceSetRequest>(message_);

  command->Init();
  command->Run();
}

TEST_F(DeleteInteractionChoiceSetResponseTest, Run_SuccessFalse_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::success] = false;

  EXPECT_CALL(app_mngr_,
              SendMessageToMobile(CheckMessageSuccess(false), false));
  command_->Run();
}

TEST_F(DeleteInteractionChoiceSetResponseTest, Run_ValidResultCode_SUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::result_code] =
      hmi_apis::Common_Result::SUCCESS;

  EXPECT_CALL(app_mngr_, SendMessageToMobile(CheckMessageSuccess(true), false));
  command_->Run();
}

TEST_F(DeleteInteractionChoiceSetResponseTest,
       Run_InvalidResultCode_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::result_code] =
      hmi_apis::Common_Result::INVALID_ENUM;

  EXPECT_CALL(app_mngr_,
              SendMessageToMobile(CheckMessageSuccess(false), false));
  command_->Run();
}

}  // namespace delete_interaction_choice_set
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
