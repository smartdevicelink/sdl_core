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

#include <string>
#include "gtest/gtest.h"
#include "rpc_base/validation_report.h"

namespace test {
using namespace rpc;

class ValidationReportTest : public testing::Test {
 protected:
  static ValidationReport* report_;
  static ValidationReport* report_2;
  static const std::string object_name_;
  static const std::string object_name_2;
  static const std::string subobject_name_1;
  static const std::string subobject_name_2;
  static const std::string subobject_name_3;
  static const std::string test_validation_info_;
  static const std::string parent_object_name_;

  static void SetUpTestCase() {
      report_ = new ValidationReport(object_name_);
      report_2 = new ValidationReport(object_name_2);
  }
  virtual void TearDown() { ClearReports(); }

  void ClearReports() {
    ValidationReports& temp =
        const_cast<ValidationReports&>(report_->subobject_reports());
    temp.clear();
  }

  void GeneratePrettyFormatResult(std::string& result, const std::string& parent_name,
                                  const std::string& obj_name, const std::string& val_info) {
    std::string temp;
    if (obj_name[0] != '[') {
        temp = ".";
    } else {
        temp = "";
    }
    result = parent_name + temp + obj_name + ":" + " " +
             val_info + "\n";
  }

  void ClearValidationInfo() {
    std::string& temp = const_cast<std::string&>(report_->validation_info());
    temp = "";
  }

  static void TearDownTestCase() {
    delete report_;
    delete report_2;
  }

  void FillReports(ValidationReports& reports_) {
    reports_.push_back(ValidationReport("test_subobject1"));
    reports_.push_back(ValidationReport("test_subobject2"));
    reports_.push_back(ValidationReport("test_subobject3"));
  }
};

ValidationReport* ValidationReportTest::report_ = NULL;
ValidationReport* ValidationReportTest::report_2 = NULL;
const std::string ValidationReportTest::object_name_ = "test_object";
const std::string ValidationReportTest::object_name_2 = "[test_object2]";
const std::string ValidationReportTest::subobject_name_1 = "test_subobject1";
const std::string ValidationReportTest::subobject_name_2 = "test_subobject2";
const std::string ValidationReportTest::subobject_name_3 = "test_subobject3";
const std::string ValidationReportTest::test_validation_info_ =
    "test_validation_info";
const std::string ValidationReportTest::parent_object_name_ = "test_parent";


TEST_F(ValidationReportTest, Ctor_and_object_name_test_ExpectDataCorrect) {
  EXPECT_EQ(object_name_, report_->object_name());
}

TEST_F(ValidationReportTest, Set_Get_Validation_Info_ExpectDataCorrect) {
  report_->set_validation_info("test_validation_info");
  EXPECT_EQ(test_validation_info_, report_->validation_info());
  ClearValidationInfo();
}

TEST_F(ValidationReportTest, Subobject_Reports_ExpectDataCorrect) {
  // Check before act
  EXPECT_EQ(0u, report_->subobject_reports().size());
  // Act
  report_->ReportSubobject(subobject_name_1).object_name();
  report_->ReportSubobject(subobject_name_2).object_name();
  report_->ReportSubobject(subobject_name_3).object_name();
  // Check after act
  EXPECT_EQ(3u, report_->subobject_reports().size());
}

TEST_F(ValidationReportTest, ReportSubobject_ExpectDataCorrect) {
  // Act and check
  EXPECT_EQ(subobject_name_1,
            report_->ReportSubobject(subobject_name_1).object_name());
  EXPECT_EQ(subobject_name_2,
            report_->ReportSubobject(subobject_name_2).object_name());
  EXPECT_EQ(subobject_name_3,
            report_->ReportSubobject(subobject_name_3).object_name());
  // Check after act
  EXPECT_EQ(3u, report_->subobject_reports().size());
}

TEST_F(ValidationReportTest, PrettyFormat_ExpectDataCorrect) {
  // Arrange
  std::string result1;
  std::string result2;
  report_->set_validation_info(test_validation_info_);
  report_2->set_validation_info(test_validation_info_);
  // Act
  impl::PrettyFormat(*report_, parent_object_name_, &result1);
  impl::PrettyFormat(*report_2, parent_object_name_, &result2);
  std::string temp1;
  GeneratePrettyFormatResult(temp1, parent_object_name_, object_name_, test_validation_info_);
  std::string temp2;
  GeneratePrettyFormatResult(temp2, parent_object_name_, object_name_2, test_validation_info_);
  // Checks
  EXPECT_EQ(temp1, result1);
  EXPECT_EQ(temp2, result2);
}

}  // namespace rpc
