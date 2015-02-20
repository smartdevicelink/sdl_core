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

#ifndef RPC_BASE_VALIDATION_REPORT_H_
#define RPC_BASE_VALIDATION_REPORT_H_

#include <string>
#include <list>

namespace rpc {

class ValidationReport;
typedef std::list<ValidationReport> ValidationReports;

class ValidationReport {
 public:
  explicit ValidationReport(const std::string& object_name);
  const std::string& object_name() const;
  const std::string& validation_info() const;
  void set_validation_info(const std::string& info);
  const ValidationReports& subobject_reports() const;
  ValidationReport& ReportSubobject(const std::string& object_name);
private:
  std::string object_name_;
  std::string validation_info_;
  ValidationReports subobject_reports_;
};

std::string PrettyFormat(const ValidationReport& report);

// Implementation

namespace impl {
inline void PrettyFormat(const ValidationReport& report,
                  const std::string& parent_path,
                  std::string* result) {
  std::string object_path = parent_path;
  if (!object_path.empty() && report.object_name()[0] != '[') {
    object_path.append(".");
  }
  object_path.append(report.object_name());
  if (!report.validation_info().empty()) {
    result->append(object_path);
    result->append(": ");
    result->append(report.validation_info());
    result->append("\n");
  }
  const ValidationReports& subreports = report.subobject_reports();
  for (ValidationReports::const_iterator i = subreports.begin(),
       end = subreports.end(); i != end; ++i) {
    PrettyFormat(*i, object_path, result);
  }
}
}  // namespace impl

inline ValidationReport::ValidationReport(const std::string& object_name)
  : object_name_(object_name) {
}

inline const std::string& ValidationReport::object_name() const {
  return object_name_;
}

inline const std::string& ValidationReport::validation_info() const {
  return validation_info_;
}

inline void ValidationReport::set_validation_info(const std::string& info) {
  validation_info_ = info;
}

inline const std::list<ValidationReport>& ValidationReport::subobject_reports() const {
  return subobject_reports_;
}

inline ValidationReport& ValidationReport::ReportSubobject(
    const std::string& object_name) {
  subobject_reports_.push_back(ValidationReport(object_name));
  return subobject_reports_.back();
}

inline std::string PrettyFormat(const ValidationReport& report) {
  std::string result;
  impl::PrettyFormat(report, "", &result);
  return result;
}

}  // namespace rpc

#endif /* RPC_BASE_VALIDATION_REPORT_H_ */
