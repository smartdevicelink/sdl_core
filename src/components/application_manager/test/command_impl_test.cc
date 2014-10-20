#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/MOBILE_API.h"

using ::testing::Return;
using ::testing::Truly;


bool MessageResultCodeIsGENERIC_ERROR(const utils::SharedPtr<smart_objects::SmartObject>& message) {
  if (!message) {
    return false;
  }
  const smart_objects::SmartObject& so = *(message.get());
  int32_t result_code = so[application_manager::strings::msg_params]
                        [application_manager::strings::result_code].asInt();
  if (mobile_apis::Result::GENERIC_ERROR
      == static_cast<mobile_apis::Result::eType>(result_code)) {
    return true;
  } else {
    return false;
  }
}

TEST(ApplicationManagerTest, SingltoneTestCheckWork) {
  application_manager::ApplicationManagerImpl* am = application_manager::ApplicationManagerImpl::instance();
  application_manager::ApplicationManagerImpl* am2 = application_manager::ApplicationManagerImpl::instance();
  ASSERT_TRUE(am == am2);
  EXPECT_CALL((*am), GetNextHMICorrelationID()).WillRepeatedly(Return(1));
  smart_objects::SmartObject* so = application_manager::MessageHelper::CreateModuleInfoSO(0);
  delete so;
  application_manager::ApplicationManagerImpl::destroy();
}

TEST(MobileCommandsTest, CommandImplTimeOut) {
  application_manager::ApplicationManagerImpl* am = application_manager::ApplicationManagerImpl::instance();
  smart_objects::SmartObject* so = application_manager::MessageHelper::CreateModuleInfoSO(0);
  application_manager::commands::CommandRequestImpl request(so);
  EXPECT_CALL((*am), ManageMobileCommand(Truly(MessageResultCodeIsGENERIC_ERROR)));
  request.onTimeOut();
  application_manager::ApplicationManagerImpl::destroy();
}
