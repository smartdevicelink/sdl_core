/*
 * Copyright (c) 2015, Ford Motor Company
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
#include "policy/access_remote_impl.h"

namespace policy {

TEST(AccessRemoteImplTest, CheckModuleType) {
  AccessRemoteImpl access_remote;
  access_remote.cache_->pt_ = new policy_table::Table();

  // No application
  EXPECT_FALSE(access_remote.CheckModuleType("1234", policy_table::MT_RADIO));

  // No modules
  policy_table::ApplicationPolicies& apps =
      access_remote.cache_->pt_->policy_table.app_policies_section.apps;
  apps["1234"];
  EXPECT_FALSE(access_remote.CheckModuleType("1234", policy_table::MT_RADIO));

  // Empty modules
  policy_table::ModuleTypes& modules = *apps["1234"].moduleType;
  modules.mark_initialized();
  EXPECT_TRUE(access_remote.CheckModuleType("1234", policy_table::MT_RADIO));
  EXPECT_TRUE(access_remote.CheckModuleType("1234", policy_table::MT_CLIMATE));

  // Specific modules
  modules.push_back(policy_table::MT_RADIO);
  EXPECT_TRUE(access_remote.CheckModuleType("1234", policy_table::MT_RADIO));
  EXPECT_FALSE(access_remote.CheckModuleType("1234", policy_table::MT_CLIMATE));
}

TEST(AccessRemoteImplTest, SetDefaultHmiTypes) {
  AccessRemoteImpl access_remote;

  std::vector<int> hmi_expected;
  hmi_expected.push_back(2);
  hmi_expected.push_back(6);
  ApplicationOnDevice who = {"dev1", "1234"};
  access_remote.SetDefaultHmiTypes(who, hmi_expected);

  EXPECT_NE(access_remote.hmi_types_.end(), access_remote.hmi_types_.find(who));
  policy_table::AppHMITypes& hmi_output = access_remote.hmi_types_[who];
  EXPECT_EQ(2u, hmi_output.size());
  EXPECT_EQ(policy_table::MEDIA, hmi_output[0]);
  EXPECT_EQ(policy_table::SOCIAL, hmi_output[1]);
}

TEST(AccessRemoteImplTest, GetGroups) {
  AccessRemoteImpl access_remote;
  ApplicationOnDevice who = {"dev1", "1234"};
  access_remote.hmi_types_[who].push_back(policy_table::REMOTE_CONTROL);

  access_remote.cache_->pt_ = new policy_table::Table();
  policy_table::ApplicationPolicies& apps =
      access_remote.cache_->pt_->policy_table.app_policies_section.apps;
  apps["1234"].groups.push_back("group_default");
  apps["1234"].AppHMIType->push_back(policy_table::MEDIA);

  // Default groups
  const policy_table::Strings& groups1 = access_remote.GetGroups(who);
  EXPECT_EQ(std::string("group_default"), std::string(groups1[0]));
}

}  // namespace policy
