/**
 * @file policy_manager.cc
 * @brief Policy manager source file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "policies/policy_manager.h"
#include "smart_objects/always_true_schema_item.h"
#include "utils/file_system.h"

namespace policies_ns = NsSmartDeviceLink::policies;
namespace so_ns = NsSmartDeviceLink::NsSmartObjects;

log4cxx::LoggerPtr policies_ns::PolicyManager::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("Policies"));

//---------------------------------------------------------------

policies_ns::PolicyManager::PolicyManager(
  const PolicyConfiguration& policy_config)
  : policy_config_(policy_config)
  , policy_table_(0) {
}

//---------------------------------------------------------------

void policies_ns::PolicyManager::Init() {
  //TODO: Implement Preload PT load logic.
  std::string pt_string;
  if (0 == policy_table_) {
    if (true == file_system::ReadFile(policy_config_.getPTFileName(),
                                    pt_string)) {
      policy_table_ = new policies_ns::PolicyTable(pt_string);
    } else {
      LOG4CXX_ERROR(logger_,
      "Can't read policy table file " << policy_config_.getPTFileName());
    }
  } else {
    LOG4CXX_WARN(logger_,
    "Policy table is already created.");

  }
}

//---------------------------------------------------------------

policies_ns::CheckPermissionResult::eType
  policies_ns::PolicyManager::checkPermission(
    uint32_t app_id, const so_ns::SmartObject& rpc) {
  return policies_ns::CheckPermissionResult::PERMISSION_OK;
}

//---------------------------------------------------------------

void policies_ns::PolicyManager::StorePolicyTable() {
  if (0 != policy_table_) {
    const std::string pt_string = policy_table_->AsString();
    const std::vector<unsigned char> char_vector_pdata(pt_string.begin(),
                                                     pt_string.end());
    if (false ==
      file_system::Write(policy_config_.getPTFileName(), char_vector_pdata)) {
      LOG4CXX_ERROR(logger_,
        "Can't write policy table file " << policy_config_.getPTFileName());
    }
  }
}
