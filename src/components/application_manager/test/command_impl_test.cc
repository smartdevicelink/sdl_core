/*
 * Copyright (c) 2014, Ford Motor Company
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

TEST(ApplicationManager, SingletonInstance_CallTwice_ReferencesAreSame) {
  application_manager::ApplicationManagerImpl* am = application_manager::ApplicationManagerImpl::instance();
  application_manager::ApplicationManagerImpl* am2 = application_manager::ApplicationManagerImpl::instance();
  ASSERT_EQ(am, am2);
  //EXPECT_CALL((*am), GetNextHMICorrelationID()).WillRepeatedly(Return(1));
  //smart_objects::SmartObjectSPtr so = application_manager::MessageHelper::CreateModuleInfoSO(0);
  application_manager::ApplicationManagerImpl::destroy();
}

TEST(MobileCommandsTest, CommandImplTimeOut) {
  application_manager::ApplicationManagerImpl* am = application_manager::ApplicationManagerImpl::instance();
  smart_objects::SmartObjectSPtr so = application_manager::MessageHelper::CreateModuleInfoSO(0);
  application_manager::commands::CommandRequestImpl request(so);
  EXPECT_CALL((*am), ManageMobileCommand(Truly(MessageResultCodeIsGENERIC_ERROR)));
  request.onTimeOut();
  application_manager::ApplicationManagerImpl::destroy();
}
