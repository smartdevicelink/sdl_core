#include <stdint.h>
#include <string>
#include <set>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "utils/custom_string.h"
#include "application_manager/mock_application.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/mobile/add_command_request.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/application_manager.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/message_helper.h"
#include "application_manager/event_engine/event.h"


namespace test {
namespace components {
namespace commands_test {

namespace am = application_manager;
using am::commands::CommandImpl;
using am::ApplicationManager;
using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using ::testing::_;
using ::utils::SharedPtr;
using ::testing::Return;


using ::test::components::application_manager_test::MockApplication;
using usage_statistics_test::MockStatisticsManager;
using am::event_engine::EventObserver;


namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace custom_str = utils::custom_string;
namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

class AddCommandRequestImplTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
};

const hmi_apis::FunctionID::eType kInvalidFunctionId_ =
    hmi_apis::FunctionID::INVALID_ENUM;

TEST_F(AddCommandRequestImplTest,  GetRunMethods_SUCCESS){
//#ifdef __UNIT_TEST__
//     friend AddCommandRequest;
//   #endif
     MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);

     smart_objects::SmartObject menu_params = smart_objects::SmartObject(smart_objects::SmartType_Map);
     menu_params[am::hmi_request::parent_id] = 1;
     menu_params[strings::position] = 10;
     menu_params[strings::menu_name] = "LG";


     smart_objects::SmartObject msg_params = smart_objects::SmartObject(smart_objects::SmartType_Map);
     msg_params[strings::cmd_id] = 1;
     msg_params[strings::menu_params] = menu_params;
     msg_params[strings::app_id] = 34;
    (*msg)[strings::msg_params] = msg_params;


     using namespace application_manager::commands;
     utils::SharedPtr<AddCommandRequest> req = CreateCommand<AddCommandRequest>(msg);

     MockAppPtr mock_app = CreateMockApp();
     ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
     EXPECT_CALL(*mock_app, FindCommand(_));
     EXPECT_CALL(*mock_app, AddCommand(_, _));

     MockMessageHelper* mock_message_helper = MockMessageHelper::message_helper_mock();

     EXPECT_CALL(*mock_message_helper,
                  VerifyImage(_, _, _));

      req->Run();
}

TEST_F(AddCommandRequestImplTest, OnEvent_SUCCESS) {

    MessageSharedPtr msg1 = CreateMessage(smart_objects::SmartType_Map);
    smart_objects::SmartObject menu_params1 = smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params1[am::hmi_request::parent_id] = 1;
    menu_params1[strings::position] = 10;
    menu_params1[strings::menu_name] = "LG";
    smart_objects::SmartObject msg_params1 = smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params1[strings::cmd_id] = 1;
    msg_params1[strings::menu_params] = menu_params1;
    msg_params1[strings::app_id] = 34;
   (*msg1)[strings::msg_params] = msg_params1;
    using namespace application_manager::commands;
    utils::SharedPtr<AddCommandRequest> req1 = CreateCommand<AddCommandRequest>(msg1);

 /*   MessageSharedPtr msg2 = CreateMessage(smart_objects::SmartType_Map);
    smart_objects::SmartObject menu_params2 = smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params2[am::hmi_request::parent_id] = 2;
    menu_params2[strings::position] = 11;
    menu_params2[strings::menu_name] = "Samsung";
    smart_objects::SmartObject msg_params2 = smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params2[strings::cmd_id] = 2;
    msg_params2[strings::menu_params] = menu_params2;
    msg_params2[strings::app_id] =22;
   (*msg2)[strings::msg_params] = msg_params2;
    utils::SharedPtr<AddCommandRequest> req2 = CreateCommand<AddCommandRequest>(msg2);

    MessageSharedPtr msg3 = CreateMessage(smart_objects::SmartType_Map);
    smart_objects::SmartObject menu_params3 = smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params3[am::hmi_request::parent_id] = 3;
    menu_params3[strings::position] = 30;
    menu_params3[strings::menu_name] = "Iphone";
    smart_objects::SmartObject msg_params3 = smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params3[strings::cmd_id] = 3;
    msg_params3[strings::menu_params] = menu_params3;
    msg_params3[strings::app_id] = 33;
   (*msg3)[strings::msg_params] = msg_params3;
    utils::SharedPtr<AddCommandRequest> req3 = CreateCommand<AddCommandRequest>(msg3);


    CommandsMap commands_map;
    commands_map.insert(CommandsMap::value_type(0u, req));
    commands_map.insert(CommandsMap::value_type(0u, req2));
    commands_map.insert(CommandsMap::value_type(0u, req3));

    ON_CALL(app_mock, commands_map())
        .WillByDefault(Return(DataAccessor<CommandsMap>(commands_map, lock_)));
*/
 /**********************************/
    MockAppPtr mock_app = CreateMockApp();
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
    EXPECT_CALL(*mock_app, app_id());


    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[strings::params][hmi_response::code] = 0;
    //static_cast<hmi_apis::Common_Result::SUCCESS>(message[strings::params][hmi_response::code].asInt());
    (*msg)[strings::params][strings::msg_params][strings::cmd_id] = 1;

    application_manager::event_engine::Event event1(hmi_apis::FunctionID::UI_AddCommand);
    event1.set_smart_object(msg);

    //creating UI-AddCommand
   /* smart_objects::SmartObjectSPtr ui_command = MessageHelper::CreateRequestObject(app_mngr_.GetNextHMICorrelationID());
    (*ui_command)[strings::params][strings::function_id] =
        static_cast<int>(hmi_apis::FunctionID::UI_AddCommand);
    smart_objects::SmartObject menu_params_UI = smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params_UI[am::hmi_request::parent_id] = 3;
    menu_params_UI[strings::position] = 30;
    menu_params_UI[strings::menu_name] = "Iphone";
    smart_objects::SmartObject msg_params_UI = smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params_UI[strings::cmd_id] = 3;
    msg_params_UI[strings::menu_params] = menu_params_UI;
    msg_params_UI[strings::app_id] = mock_app->app_id();
   (*ui_command)[strings::msg_params] = msg_params_UI;

    smart_objects::SmartObjectList requests = MessageHelper::CreateAddCommandRequestToHMI(mock_app, app_mngr_);
    requests.push_back(ui_command);

  application_manager::event_engine::Event event2(hmi_apis::Common_Result::SUCCESS);
  event2.set_smart_object(req2);
  EXPECT_NO_THROW(req2->on_event(event2));

  application_manager::event_engine::Event event3(hmi_apis::Common_Result::SUCCESS);
  event3.set_smart_object(req3);
  EXPECT_NO_THROW(req3->on_event(event3));*/


   EXPECT_CALL(*mock_app, UpdateHash());


   EXPECT_NO_THROW(req1->on_event(event1));
   /******************************************************************/

}



} // namespace tests
} //namespace components
} //namespace commands_test

