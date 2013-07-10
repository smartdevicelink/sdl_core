#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock.h"
#ifndef BUILD_ONLY_TM
#include "utils/file_system_tests.h"
#include "protocol_handler/protocol_handler_tm_test.h"
#include "request_watchdog/request_watchdog_test.h"
#include "application_manager/formatters_commands.h"
#include "SmartObjectDraftTest.h"
#include "SmartObjectInvalidTest.h"
#include "SmartObjectStressTest.h"
#include "SmartObjectUnitTest.h"
#include "TSharedPtrTest.h"
<<<<<<< HEAD
#endif


=======
#include "mobile_message_handler/mobile_message_handler_test.h"
>>>>>>> origin/develop
//#include "SmartObjectConvertionTimeTest.h"
//#include "jsoncpp/json_reader_test.h"
//#include "json_handler/smart_schema_draft_test.h"
//#include "json_handler/formatters/formatter_test_helper.h"
//#include "json_handler/formatters/formatter_json_alrpcv1_test.h"
//#include "json_handler/formatters/formatter_json_alrpcv2_test.h"
//#include "json_handler/formatters/formatter_json_rpcv2_test.h"

<<<<<<< HEAD
#ifndef BUILD_ONLY_TM
=======

using namespace test::components::mobile_message_handler_test;
>>>>>>> origin/develop
using namespace test::components::request_watchdog_test;
using namespace test::components::utils;
using namespace test::components::protocol_handler_test;
using namespace test::components::SmartObjects;
using namespace utils;
#endif
//using namespace test::third_party_libs::json_reader_test;
//using namespace test::components::json_handler::smart_schema_draft_test;
//using namespace test::components::json_handler::formatters;

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
