#include "gtest/gtest.h"
#include "gmock/gmock.h"
//#include "config_profile/profile.h"
#include "utils/file_system_tests.h"
#include "utils/prioritized_queue_tests.h"
#include "protocol_handler/protocol_handler_tm_test.h"
//#include "request_watchdog/request_watchdog_test.h"
#include "application_manager/formatters_commands.h"
//#include "media_manager/media_manager_impl_test.h"
//#include "SmartObjectDraftTest.h"
//#include "SmartObjectInvalidTest.h"
//#include "SmartObjectStressTest.h"
//#include "SmartObjectUnitTest.h"
//#include "TSharedPtrTest.h"
//#include "SmartObjectConvertionTimeTest.h"
//#include "jsoncpp/json_reader_test.h"
//#include "json_handler/smart_schema_draft_test.h"
//#include "json_handler/formatters/formatter_test_helper.h"
//#include "json_handler/formatters/formatter_json_alrpcv1_test.h"
//#include "json_handler/formatters/formatter_json_alrpcv2_test.h"
//#include "json_handler/formatters/formatter_json_rpcv2_test.h"


#include "rpc/AdminAppTest.h"
//#include "smart_objects/smart_object.h"
//#include "application_manager/smart_object_keys.h"
//#include "formatters/CSmartFactory.hpp"

//using namespace test::components::request_watchdog_test;
//using namespace test::components::utils;
//using namespace test::components::protocol_handler_test;
//using namespace utils;

//using namespace test::third_party_libs::json_reader_test;
//using namespace test::components::json_handler::smart_schema_draft_test;
//using namespace test::components::json_handler::formatters;

//using namespace NsSmartDeviceLink::NsSmartObjects;
//using namespace NsSmartDeviceLink::NsJSONHandler::strings;

#ifdef __cplusplus
extern "C" void __gcov_flush();
#endif

int main(int argc, char **argv) {printf("\n\n\n after init in Admin \n\n\n");
  ::testing::InitGoogleMock(&argc, argv);

  profile::Profile::instance()->config_file_name("smartDeviceLink.ini");

	application_manager::ApplicationManagerImpl* app_manager_ = 
										application_manager::ApplicationManagerImpl::instance();
										
	printf("\n\n\n after init in Admin \n\n\n");
  //test::app_manager_test::AdminAppTest app;
 
  //threads::Thread threadAdminAppTest("AdminAppTest", &app);

  //threadAdminAppTest.start();

  //app.run();
  
  //threadAdminAppTest.join();
  //sleep(60);

  //app.run();

  //sleep(20);

  //app.run();
  
  //threadAdminAppTest.join();
  //sleep(60);
  

  int result = RUN_ALL_TESTS();

#ifdef __cplusplus
  __gcov_flush();
#endif

  return result;
}


