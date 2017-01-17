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
#include <stdint.h>
#include <set>

#include "gtest/gtest.h"

#include "application_manager/application_manager_impl.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_resume_ctrl.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "media_manager/mock_media_manager.h"
#include "resumption/last_state_impl.h"
#include "policy/mock_policy_settings.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "utils/file_system.h"

namespace am = application_manager;
namespace policy_test = test::components::policy_handler_test;
using ::test::components::application_manager_test::MockApplication;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;

namespace resumption {
class LastState;
}

namespace test {
namespace components {
namespace application_manager_test {

namespace {
const std::string kAppStorageFolder("./storage");
const std::string kAppInfoStorage("info");
const uint32_t kThreadPoolSize = 1u;
const uint32_t kAppId = 10u;
const bool kHmiLaunch = false;
const uint32_t kStopStreamingTimeout = 10000u;
const std::string kFileName = "file_name";
}  // namespace

class ApplicationManagerImplTest : public ::testing::Test {
 public:
  void SetUp() OVERRIDE {
    ON_CALL(mock_application_manager_settings_, thread_pool_size())
        .WillByDefault(Return(kThreadPoolSize));
    ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
        .WillByDefault(ReturnRef(kFileName));
    ON_CALL(mock_application_manager_settings_, use_db_for_resumption())
        .WillByDefault(Return(false));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(kHmiLaunch));
    ON_CALL(mock_application_manager_settings_, stop_streaming_timeout())
        .WillByDefault(Return(kStopStreamingTimeout));
    ON_CALL(mock_application_manager_settings_,
            app_resumption_save_persistent_data_timeout())
        .WillByDefault(ReturnRef(kAppId));
    ON_CALL(mock_application_manager_settings_, max_number_of_ios_device())
        .WillByDefault(Return(0));
    EXPECT_CALL(mock_policy_settings_, enable_policy())
        .WillOnce(Return(true))
        .WillOnce(Return(false));

    file_system::CreateDirectory(kAppStorageFolder);

    app_manager_impl_ = utils::MakeShared<am::ApplicationManagerImpl>(
        mock_application_manager_settings_, mock_policy_settings_);
    last_state_ = utils::MakeShared<resumption::LastStateImpl>(kAppStorageFolder,
                                                           kAppInfoStorage);
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(file_system::RemoveDirectory(kAppStorageFolder, true));
  }

  resumprion_test::MockResumeCtrl mock_resume_ctrl_;
  policy_test::MockPolicyHandlerInterface mock_policy_handler_interface_;
  MockHMICapabilities mock_hmi_capabilities_;
  media_manager_test::MockMediaManager mock_media_manager_;
  MockApplicationManagerSettings mock_application_manager_settings_;
  policy_handler_test::MockPolicySettings mock_policy_settings_;
  utils::SharedPtr<resumption::LastState> last_state_;
  utils::SharedPtr<am::ApplicationManagerImpl> app_manager_impl_;
};

TEST_F(ApplicationManagerImplTest,
       AppManagerInit_IsReadWriteAllowedAppIconFolder_SUCCESS) {
  EXPECT_CALL(mock_application_manager_settings_, app_storage_folder())
      .WillRepeatedly(ReturnRef(kAppStorageFolder));

  EXPECT_CALL(mock_application_manager_settings_, system_files_path())
      .WillOnce(ReturnRef(kAppStorageFolder));
  EXPECT_CALL(mock_application_manager_settings_, app_icons_folder())
      .WillOnce(ReturnRef(kAppStorageFolder));

  EXPECT_FALSE(
      app_manager_impl_->Init(*(last_state_.get()), &mock_media_manager_));
}

}  // application_manager_test
}  // namespace components
}  // namespace test
