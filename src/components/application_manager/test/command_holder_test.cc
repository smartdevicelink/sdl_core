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

#include <gmock/gmock.h>

#include "application_manager/command_holder_impl.h"
#include "smart_objects/smart_object.h"
#include "utils/shared_ptr.h"

#include "application_manager/mock_application_manager.h"

namespace test {
namespace components {
namespace application_manager_test {

using testing::_;

namespace am = application_manager;

class CommandHolderImplTest : public testing::Test {
 public:
  CommandHolderImplTest()
      : cmd_ptr_(new smart_objects::SmartObject), kPolicyAppId_("p_app_id") {}

  MockApplicationManager mock_app_manager_;
  utils::SharedPtr<smart_objects::SmartObject> cmd_ptr_;
  const std::string kPolicyAppId_;
};

TEST_F(CommandHolderImplTest, HoldOne_ExpectReleaseOne) {
  am::CommandHolderImpl cmd_holder(mock_app_manager_);
  cmd_holder.Suspend(kPolicyAppId_, cmd_ptr_);

  // Act
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(cmd_ptr_));
  cmd_holder.Resume(kPolicyAppId_);
}

TEST_F(CommandHolderImplTest, HoldMany_ExpectReleaseSame) {
  am::CommandHolderImpl cmd_holder(mock_app_manager_);

  int32_t iterations = 0;
  do {
    cmd_holder.Suspend(kPolicyAppId_, cmd_ptr_);
    ++iterations;
  } while (iterations < 5);

  // Act
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(cmd_ptr_)).Times(iterations);
  cmd_holder.Resume(kPolicyAppId_);
}

TEST_F(CommandHolderImplTest, Hold_Drop_ExpectNoReleased) {
  am::CommandHolderImpl cmd_holder(mock_app_manager_);
  cmd_holder.Suspend(kPolicyAppId_, cmd_ptr_);
  cmd_holder.Suspend(kPolicyAppId_, cmd_ptr_);

  // Act
  cmd_holder.Clear(kPolicyAppId_);
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(cmd_ptr_)).Times(0);
  cmd_holder.Resume(kPolicyAppId_);
}

TEST_F(CommandHolderImplTest, Hold_ReleaseAnotherId_ExpectNoReleased) {
  am::CommandHolderImpl cmd_holder(mock_app_manager_);
  cmd_holder.Suspend(kPolicyAppId_, cmd_ptr_);
  cmd_holder.Suspend(kPolicyAppId_, cmd_ptr_);

  // Act
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(cmd_ptr_)).Times(0);
  cmd_holder.Resume("anotherId");
}

TEST_F(CommandHolderImplTest, Hold_DropAnotherId_ExpectReleased) {
  am::CommandHolderImpl cmd_holder(mock_app_manager_);

  int32_t iterations = 0;
  do {
    cmd_holder.Suspend(kPolicyAppId_, cmd_ptr_);
    ++iterations;
  } while (iterations < 3);

  // Act
  cmd_holder.Clear("anotherId");

  EXPECT_CALL(mock_app_manager_, ManageHMICommand(cmd_ptr_)).Times(iterations);
  cmd_holder.Resume(kPolicyAppId_);
}

}  // application_manager_test
}  // components
}  // test
