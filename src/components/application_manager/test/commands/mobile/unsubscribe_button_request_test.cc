#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/commands/mobile/unsubscribe_button_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unsubscribe_button_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;

using am::commands::UnsubscribeButtonRequest;
using am::commands::MessageSharedPtr;

typedef ::utils::SharedPtr<UnsubscribeButtonRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const mobile_apis::ButtonName::eType kButtonId = mobile_apis::ButtonName::OK;
}  // namespace

class UnsubscribeButtonRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(UnsubscribeButtonRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<UnsubscribeButtonRequest>());

  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(UnsubscribeButtonRequestTest,
       Run_UnsubscribeNotSubscribedButton_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::strings::button_name] = kButtonId;

  CommandPtr command(CreateCommand<UnsubscribeButtonRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, UnsubscribeFromButton(kButtonId))
      .WillOnce(Return(false));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  command->Run();
}

TEST_F(UnsubscribeButtonRequestTest, Run_SUCCESS) {
  const mobile_apis::ButtonName::eType kButtonId = mobile_apis::ButtonName::OK;

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::strings::button_name] = kButtonId;

  CommandPtr command(CreateCommand<UnsubscribeButtonRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, UnsubscribeFromButton(kButtonId))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::Buttons_OnButtonSubscription)));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  EXPECT_CALL(*mock_app, UpdateHash());

  command->Run();
}

}  // namespace unsubscribe_button_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
