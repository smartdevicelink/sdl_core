/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "application_manager/usage_statistics.h"
#include <memory>
#include "gmock/gmock.h"
#include "smart_objects/enum_schema_item.h"
#include "mock_statistics_manager.h"
#include "mock_app_stopwatch.h"
#include "utils/make_shared.h"
#include "utils/shared_ptr.h"

namespace test {
namespace components {
namespace usage_statistics_test {

using namespace mobile_apis;                        // For Language enums
using namespace NsSmartDeviceLink::NsSmartObjects;  // For EnumToCString &
                                                    // EnumConversionHelper
using namespace usage_statistics;
using testing::_;

namespace {

std::string LanguageIdToString(Language::eType lang_id) {
  const char* str;
  const bool ok =
      EnumConversionHelper<Language::eType>::EnumToCString(lang_id, &str);
  return ok ? str : "unknown";
}

// Constant values used in tests
const Language::eType kTestLanguageId = Language::eType::DE_DE;
const usage_statistics::AppStopwatchId kTestAppStopwatchId =
    usage_statistics::AppStopwatchId::SECONDS_HMI_NONE;
const std::string kAppId = "SPT";

}  // namespace

class UsageStatisticsTest : public testing::Test {
 public:
  UsageStatisticsTest()
      : mock_statistics_manager_sptr_(
            utils::MakeShared<MockStatisticsManager>())
      , usage_statistics_test_object1_sptr_(
            new application_manager::UsageStatistics(
                kAppId, mock_statistics_manager_sptr_))
      , language_(LanguageIdToString(kTestLanguageId)) {}

 protected:
  utils::SharedPtr<MockStatisticsManager>
      mock_statistics_manager_sptr_;
  std::auto_ptr<application_manager::UsageStatistics>
      usage_statistics_test_object1_sptr_;
  const std::string language_;
};

TEST_F(UsageStatisticsTest, RecordHmiStateChanged_CallMethod_ExpectMethodCall) {
  // Arrange
  std::auto_ptr<MockAppStopwatch> mock_app_stopwatch_object(new MockAppStopwatch);

  // Checks
  EXPECT_CALL(*mock_app_stopwatch_object, Start(kTestAppStopwatchId));
  EXPECT_CALL(*mock_app_stopwatch_object, Switch(kTestAppStopwatchId));

  // Act
  std::auto_ptr<application_manager::UsageStatistics>
      usage_statistics_test_object2_sptr_(
          new application_manager::UsageStatistics(
              kAppId,
              mock_statistics_manager_sptr_,
              mock_app_stopwatch_object.release()));
  usage_statistics_test_object2_sptr_->RecordHmiStateChanged(
      HMILevel::eType::HMI_NONE);
}

TEST_F(UsageStatisticsTest,
       RecordAppRegistrationGuiLanguage_CallMethod_ExpectSetMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Set(kAppId, _, language_));
  // Act
  usage_statistics_test_object1_sptr_->RecordAppRegistrationGuiLanguage(
      kTestLanguageId);
}

TEST_F(UsageStatisticsTest,
       RecordAppRegistrationVuiLanguage_CallMethod_ExpectSetMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Set(kAppId, _, language_));
  // Act
  usage_statistics_test_object1_sptr_->RecordAppRegistrationVuiLanguage(
      kTestLanguageId);
}

TEST_F(UsageStatisticsTest,
       RecordRpcSentInHMINone_CallMethod_ExpectIncrementMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Increment(kAppId, _));
  // Act
  usage_statistics_test_object1_sptr_->RecordRpcSentInHMINone();
}

TEST_F(UsageStatisticsTest,
       RecordPolicyRejectedRpcCall_CallMethod_ExpectIncrementMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Increment(kAppId, _));
  // Act
  usage_statistics_test_object1_sptr_->RecordPolicyRejectedRpcCall();
}

TEST_F(UsageStatisticsTest,
       RecordAppUserSelection_CallMethod_ExpectIncrementMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Increment(kAppId, _));
  // Act
  usage_statistics_test_object1_sptr_->RecordAppUserSelection();
}

TEST_F(UsageStatisticsTest,
       RecordRunAttemptsWhileRevoked_CallMethod_ExpectIncrementMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Increment(kAppId, _));
  // Act
  usage_statistics_test_object1_sptr_->RecordRunAttemptsWhileRevoked();
}

TEST_F(UsageStatisticsTest,
       RecordRemovalsForBadBehavior_CallMethod_ExpectIncrementMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Increment(kAppId, _));
  // Act
  usage_statistics_test_object1_sptr_->RecordRemovalsForBadBehavior();
}

TEST_F(UsageStatisticsTest,
       RecordTLSError_CallMethod_ExpectIncrementMethodCall) {
  // Expectation
  EXPECT_CALL(*mock_statistics_manager_sptr_, Increment(kAppId, _));
  // Act
  usage_statistics_test_object1_sptr_->RecordTLSError();
}

}  // namespace usage_statistics_test
}  // namespace components
}  // namespace test
