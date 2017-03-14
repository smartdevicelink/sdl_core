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

TEST(AccessRemoteImplTest, Allow) {
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  SeatLocation zone = {0, 0, 0};
  Object what = {policy_table::MT_RADIO, zone};
  access_remote.Allow(who, what);
  AccessRemoteImpl::AccessControlList::const_iterator i =
      access_remote.acl_.find(what);
  ASSERT_NE(access_remote.acl_.end(), i);
  AccessRemoteImpl::AccessControlRow::const_iterator j = i->second.find(who);
  ASSERT_NE(i->second.end(), j);
  EXPECT_EQ(TypeAccess::kAllowed, j->second);
}

TEST(AccessRemoteImplTest, KeyMapTest) {
  // Testing operator < to use as key of map
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  SeatLocation zone = {0, 0, 0};
  Object what1 = {policy_table::MT_RADIO, zone};
  Object what2 = {policy_table::MT_CLIMATE, zone};
  access_remote.Allow(who, what1);
  access_remote.Allow(who, what2);
  ASSERT_EQ(2u, access_remote.acl_.size());
}

TEST(AccessRemoteImplTest, Deny) {
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  SeatLocation zone = {0, 0, 0};
  Object what = {policy_table::MT_RADIO, zone};
  access_remote.Deny(who, what);
  AccessRemoteImpl::AccessControlList::const_iterator i =
      access_remote.acl_.find(what);
  ASSERT_NE(access_remote.acl_.end(), i);
  AccessRemoteImpl::AccessControlRow::const_iterator j = i->second.find(who);
  ASSERT_NE(i->second.end(), j);
  EXPECT_EQ(TypeAccess::kDisallowed, j->second);
}

TEST(AccessRemoteImplTest, ChangeAccess) {
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  SeatLocation zone = {0, 0, 0};
  Object what = {policy_table::MT_RADIO, zone};
  access_remote.Allow(who, what);
  ASSERT_EQ(TypeAccess::kAllowed, access_remote.acl_[what][who]);
  access_remote.Deny(who, what);
  ASSERT_EQ(TypeAccess::kDisallowed, access_remote.acl_[what][who]);
  access_remote.Allow(who, what);
  EXPECT_EQ(TypeAccess::kAllowed, access_remote.acl_[what][who]);
}

TEST(AccessRemoteImplTest, ResetBySubject) {
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  SeatLocation zone = {0, 0, 0};
  Object what1 = {policy_table::MT_RADIO, zone};
  Object what2 = {policy_table::MT_CLIMATE, zone};
  access_remote.Allow(who, what1);
  access_remote.Deny(who, what2);
  ASSERT_EQ(2u, access_remote.acl_.size());
  ASSERT_EQ(1u, access_remote.acl_[what1].size());
  ASSERT_EQ(1u, access_remote.acl_[what2].size());

  access_remote.Reset(who);
  ASSERT_EQ(2u, access_remote.acl_.size());
  EXPECT_TRUE(access_remote.acl_[what1].empty());
  EXPECT_TRUE(access_remote.acl_[what2].empty());
}

TEST(AccessRemoteImplTest, ResetByObject) {
  AccessRemoteImpl access_remote;
  Subject who1 = {"dev1", "12345"};
  Subject who2 = {"dev2", "123456"};
  SeatLocation zone = {0, 0, 0};
  Object what = {policy_table::MT_RADIO, zone};
  access_remote.Allow(who1, what);
  access_remote.Deny(who2, what);
  ASSERT_EQ(1u, access_remote.acl_.size());
  ASSERT_EQ(2u, access_remote.acl_[what].size());

  access_remote.Reset(what);
  EXPECT_TRUE(access_remote.acl_.empty());
}

TEST(AccessRemoteImplTest, CheckAllowed) {
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  SeatLocation zone = {0, 0, 0};
  Object what = {policy_table::MT_RADIO, zone};
  access_remote.Allow(who, what);

  EXPECT_EQ(TypeAccess::kAllowed, access_remote.Check(who, what));
}

TEST(AccessRemoteImplTest, CheckDisallowed) {
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  Subject who1 = {"dev1", "123456"};
  SeatLocation zone = {0, 0, 0};
  Object what = {policy_table::MT_RADIO, zone};

  access_remote.Allow(who, what);
  EXPECT_EQ(TypeAccess::kManual, access_remote.Check(who1, what));

  access_remote.Reset(who);
  access_remote.Deny(who1, what);
  EXPECT_EQ(TypeAccess::kDisallowed, access_remote.Check(who1, what));
}

TEST(AccessRemoteImplTest, CheckManual) {
  AccessRemoteImpl access_remote;
  Subject who = {"dev1", "12345"};
  Subject who1 = {"dev1", "123456"};
  SeatLocation zone = {0, 0, 0};
  Object what = {policy_table::MT_RADIO, zone};

  EXPECT_EQ(TypeAccess::kManual, access_remote.Check(who, what));

  access_remote.Deny(who1, what);
  EXPECT_EQ(TypeAccess::kManual, access_remote.Check(who, what));
}

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

TEST(AccessRemoteImplTest, EnableDisable) {
  AccessRemoteImpl access_remote;
  access_remote.cache_->pt_ = new policy_table::Table();
  policy_table::ModuleConfig& config =
      access_remote.cache_->pt_->policy_table.module_config;

  // Country is enabled
  access_remote.enabled_ = true;
  *config.country_consent_passengersRC = true;
  access_remote.Enable();
  EXPECT_TRUE(*config.user_consent_passengersRC);
  EXPECT_TRUE(*config.country_consent_passengersRC);
  EXPECT_TRUE(access_remote.IsEnabled());

  access_remote.Disable();
  EXPECT_FALSE(*config.user_consent_passengersRC);
  EXPECT_TRUE(*config.country_consent_passengersRC);
  EXPECT_FALSE(access_remote.IsEnabled());

  // Country is disabled
  access_remote.enabled_ = false;
  *config.country_consent_passengersRC = false;
  access_remote.Enable();
  EXPECT_TRUE(*config.user_consent_passengersRC);
  EXPECT_FALSE(*config.country_consent_passengersRC);
  EXPECT_FALSE(access_remote.IsEnabled());

  access_remote.Disable();
  EXPECT_FALSE(*config.user_consent_passengersRC);
  EXPECT_FALSE(*config.country_consent_passengersRC);
  EXPECT_FALSE(access_remote.IsEnabled());
}

TEST(AccessRemoteImplTest, SetDefaultHmiTypes) {
  AccessRemoteImpl access_remote;

  std::vector<int> hmi_expected;
  hmi_expected.push_back(2);
  hmi_expected.push_back(6);
  Subject who = {"dev1", "1234"};
  access_remote.SetDefaultHmiTypes(who, hmi_expected);

  EXPECT_NE(access_remote.hmi_types_.end(), access_remote.hmi_types_.find(who));
  policy_table::AppHMITypes& hmi_output = access_remote.hmi_types_[who];
  EXPECT_EQ(2u, hmi_output.size());
  EXPECT_EQ(policy_table::AHT_MEDIA, hmi_output[0]);
  EXPECT_EQ(policy_table::AHT_SOCIAL, hmi_output[1]);
}

TEST(AccessRemoteImplTest, GetGroups) {
  AccessRemoteImpl access_remote;
  access_remote.primary_device_ = "dev1";
  access_remote.enabled_ = true;
  Subject who = {"dev1", "1234"};
  access_remote.hmi_types_[who].push_back(policy_table::AHT_REMOTE_CONTROL);

  access_remote.cache_->pt_ = new policy_table::Table();
  policy_table::ApplicationPolicies& apps =
      access_remote.cache_->pt_->policy_table.app_policies_section.apps;
  apps["1234"].groups.push_back("group_default");
  apps["1234"].groups_nonPrimaryRC->push_back("group_non_primary");
  apps["1234"].groups_primaryRC->push_back("group_primary");
  apps["1234"].AppHMIType->push_back(policy_table::AHT_MEDIA);

  // Default groups
  const policy_table::Strings& groups1 = access_remote.GetGroups(who);
  EXPECT_EQ(std::string("group_default"), std::string(groups1[0]));

  // Primary groups
  apps["1234"].set_to_string(policy::kDefaultId);
  const policy_table::Strings& groups2 = access_remote.GetGroups(who);
  EXPECT_EQ(std::string("group_primary"), std::string(groups2[0]));

  // Non primary groups
  apps["1234"].set_to_string(policy::kDefaultId);
  Subject who2 = {"dev2", "1234"};
  access_remote.hmi_types_[who2].push_back(policy_table::AHT_REMOTE_CONTROL);
  const policy_table::Strings& groups3 = access_remote.GetGroups(who2);
  EXPECT_EQ(std::string("group_non_primary"), std::string(groups3[0]));

  // Empty groups
  access_remote.enabled_ = false;
  apps["1234"].set_to_string(policy::kDefaultId);
  const policy_table::Strings& groups4 = access_remote.GetGroups(who2);
  EXPECT_TRUE(groups4.empty());
}

TEST(AccessRemoteImplTest, CheckParameters) {
  AccessRemoteImpl access_remote;
  access_remote.cache_->pt_ = new policy_table::Table();
  access_remote.cache_->pt_->policy_table.module_config.equipment
      ->mark_initialized();

  policy_table::Zones& zones =
      access_remote.cache_->pt_->policy_table.module_config.equipment->zones;

  zones["Block A"].col = 0;
  zones["Block A"].row = 0;
  zones["Block A"].level = 0;
  zones["Block A"].auto_allow["RADIO"];
  zones["Block A"].driver_allow["CLIMATE"];

  zones["Block B"].col = 1;
  zones["Block B"].row = 1;
  zones["Block B"].level = 1;
  zones["Block B"].auto_allow["RADIO"]["Rpc 1"];
  zones["Block B"].auto_allow["RADIO"]["Rpc 4"].push_back("param 1");
  zones["Block B"].auto_allow["RADIO"]["Rpc 4"].push_back("param 2");
  zones["Block B"].auto_allow["RADIO"]["Rpc 5"].push_back("param 1");
  zones["Block B"].driver_allow["CLIMATE"]["Rpc 2"];
  zones["Block B"].driver_allow["CLIMATE"]["Rpc 3"].push_back("param 1");
  zones["Block B"].driver_allow["CLIMATE"]["Rpc 3"].push_back("param 2");
  zones["Block B"].driver_allow["RADIO"]["Rpc 5"].push_back("param 2");

  // No zone
  SeatLocation no_zone = {2, 2, 2};
  Object what_no = {policy_table::MT_RADIO, no_zone};
  EXPECT_EQ(
      TypeAccess::kDisallowed,
      access_remote.CheckParameters(what_no, "Any rpc", RemoteControlParams()));

  // No module
  SeatLocation block_a = {0, 0, 0};
  Object what_a = {policy_table::ModuleType(-1), block_a};
  EXPECT_EQ(
      TypeAccess::kDisallowed,
      access_remote.CheckParameters(what_a, "Any rpc", RemoteControlParams()));

  // Driver allow, all RPCs are allowed
  Object what_ca = {policy_table::MT_CLIMATE, block_a};
  EXPECT_EQ(
      TypeAccess::kManual,
      access_remote.CheckParameters(what_ca, "Any rpc", RemoteControlParams()));

  // Driver allow, no RPC
  SeatLocation block_b = {1, 1, 1};
  Object what_cb = {policy_table::MT_CLIMATE, block_b};
  EXPECT_EQ(
      TypeAccess::kDisallowed,
      access_remote.CheckParameters(what_cb, "No rpc", RemoteControlParams()));

  // Driver allow, Rpc 2, all parameters are allowed
  RemoteControlParams params;
  params.push_back("param 1");
  EXPECT_EQ(TypeAccess::kManual,
            access_remote.CheckParameters(what_cb, "Rpc 2", params));

  // Driver allow, Rpc 3, empty input parameters
  EXPECT_EQ(
      TypeAccess::kDisallowed,
      access_remote.CheckParameters(what_cb, "Rpc 3", RemoteControlParams()));

  // Driver allow, Rpc 3, param 1
  RemoteControlParams params_0;
  params_0.push_back("param 1");
  EXPECT_EQ(TypeAccess::kManual,
            access_remote.CheckParameters(what_cb, "Rpc 3", params_0));

  // Driver allow, Rpc 3, param 1 and 2
  RemoteControlParams params_1;
  params_1.push_back("param 1");
  params_1.push_back("param 2");
  EXPECT_EQ(TypeAccess::kManual,
            access_remote.CheckParameters(what_cb, "Rpc 3", params_1));

  // Driver allow, Rpc 3, param 1 and 2 and 3
  RemoteControlParams params_2;
  params_2.push_back("param 1");
  params_2.push_back("param 2");
  params_2.push_back("param 3");
  EXPECT_EQ(TypeAccess::kDisallowed,
            access_remote.CheckParameters(what_cb, "Rpc 3", params_2));

  // Driver allow, Rpc 3, param 1 and 2 and 3
  RemoteControlParams params_3;
  params_3.push_back("param 3");
  EXPECT_EQ(TypeAccess::kDisallowed,
            access_remote.CheckParameters(what_cb, "Rpc 3", params_3));

  // Auto allow, Rpc 4, empty input parameters
  Object what_rb = {policy_table::MT_RADIO, block_b};
  EXPECT_EQ(
      TypeAccess::kDisallowed,
      access_remote.CheckParameters(what_rb, "Rpc 4", RemoteControlParams()));

  // Auto allow, Rpc 4, param 1
  EXPECT_EQ(TypeAccess::kAllowed,
            access_remote.CheckParameters(what_rb, "Rpc 4", params_0));

  // Auto allow, Rpc 4, param 1 and 2
  EXPECT_EQ(TypeAccess::kAllowed,
            access_remote.CheckParameters(what_rb, "Rpc 4", params_1));

  // Auto allow, Rpc 4, param 1 and 2 and 3
  EXPECT_EQ(TypeAccess::kDisallowed,
            access_remote.CheckParameters(what_rb, "Rpc 4", params_2));

  // Auto allow, Rpc 4, param 1 and 2 and 3
  EXPECT_EQ(TypeAccess::kDisallowed,
            access_remote.CheckParameters(what_rb, "Rpc 4", params_3));

  // Auto allow, Rpc 5, empty input parameters
  EXPECT_EQ(
      TypeAccess::kDisallowed,
      access_remote.CheckParameters(what_rb, "Rpc 5", RemoteControlParams()));

  // Auto allow, Rpc 5, param 1
  EXPECT_EQ(TypeAccess::kAllowed,
            access_remote.CheckParameters(what_rb, "Rpc 5", params_0));

  // Auto allow, Rpc 5, param 2
  RemoteControlParams params_12;
  params_12.push_back("param 2");
  EXPECT_EQ(TypeAccess::kManual,
            access_remote.CheckParameters(what_rb, "Rpc 5", params_12));

  // Auto allow, Rpc 5, param 1 and 2
  EXPECT_EQ(TypeAccess::kManual,
            access_remote.CheckParameters(what_rb, "Rpc 5", params_1));

  // Auto allow, Rpc 5, param 1 and 2 and 3
  EXPECT_EQ(TypeAccess::kDisallowed,
            access_remote.CheckParameters(what_rb, "Rpc 5", params_2));

  // Auto allow, Rpc 5, param 3
  EXPECT_EQ(TypeAccess::kDisallowed,
            access_remote.CheckParameters(what_rb, "Rpc 5", params_3));
}

TEST(AccessRemoteImplTest, GetDeviceZone) {
  AccessRemoteImpl access_remote;

  // No zone
  const SeatLocation* zone1 = access_remote.GetDeviceZone("dev_1");
  EXPECT_EQ(0, zone1);

  // Device has zone
  SeatLocation expect = {0, 1, 0};
  access_remote.seats_["dev_2"] = expect;
  const SeatLocation* zone2 = access_remote.GetDeviceZone("dev_2");
  EXPECT_EQ(expect, *zone2);
}

}  // namespace policy
