/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TEST_COMPONENTS_APPLICATION_MANAGER_POLICIES_TABLE_INCLUDE_POLICIES_TABLE_POLICIES_TABLE_TEST_H_
#define TEST_COMPONENTS_APPLICATION_MANAGER_POLICIES_TABLE_INCLUDE_POLICIES_TABLE_POLICIES_TABLE_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "application_manager/policies_manager/policies_manager.h"

TEST(policies_manager_test, class_test) {
  application_manager::policies_manager::PoliciesManager manager;
  ASSERT_TRUE(manager.Init());
  ASSERT_TRUE(manager.IsValidHmiStatus(mobile_apis::FunctionID::RegisterAppInterfaceID, mobile_apis::HMILevel::HMI_NONE));
  ASSERT_FALSE(manager.IsValidHmiStatus(mobile_apis::FunctionID::AddSubMenuID, mobile_apis::HMILevel::HMI_NONE));
}

#endif  // TEST_COMPONENTS_APPLICATION_MANAGER_POLICIES_TABLE_INCLUDE_POLICIES_TABLE_POLICIES_TABLE_TEST_H_
