/**
 * @file policy_manager_impl.cc
 * @brief Policy Manager implementation source file.
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

#include "policies/policy_manager_impl.h"
#include "policies/policy_table_schema.h"
#include "policies/permissions_calculator.h"
#include "smart_objects/always_true_schema_item.h"
#include "utils/file_system.h"

namespace policies_ns = NsSmartDeviceLink::policies;
namespace so_ns = NsSmartDeviceLink::NsSmartObjects;

using ::policies_ns::PermissionsCalculator;

log4cxx::LoggerPtr policies_ns::PolicyManagerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("Policies"));

//---------------------------------------------------------------

policies_ns::PolicyManagerImpl::PolicyManagerImpl(
    const PolicyConfiguration& policy_config)
  : PolicyManager()
  , policy_config_(policy_config)
  , policy_table_(0) {
}

//---------------------------------------------------------------

policies_ns::PolicyManagerImpl::~PolicyManagerImpl() {
  StorePolicyTable();
  if (0 != policy_table_) {
    delete policy_table_;
    policy_table_ = 0;
  }
}

//---------------------------------------------------------------

policies_ns::InitResult::eType policies_ns::PolicyManagerImpl::Init() {
  // TODO(anyone): Provide some mechanism for recovery (from Preload???)
  // if PT file corrupted (e.g. bad json)

  InitResult::eType init_result = InitResult::INIT_FAILED_PRELOAD_NO_FILE;

  std::string pt_string;
  if (0 == policy_table_) {
    if (true == file_system::ReadFile(policy_config_.getPTFileName(),
                                    pt_string)) {
      policy_table_ = new policies_ns::PolicyTable(pt_string,
              policies_ns::PTType::TYPE_PT);
      init_result = InitResult::INIT_OK;
    } else {
      LOG4CXX_WARN(logger_, "Can't read policy table file "
          << policy_config_.getPTFileName());
      if (true == file_system::ReadFile(
          policy_config_.getPreloadPTFileName(), pt_string)) {
        policy_table_ = new policies_ns::PolicyTable(
            pt_string, policies_ns::PTType::TYPE_PRELOAD);
        init_result = InitResult::INIT_OK_PRELOAD;
      } else {
        init_result = InitResult::INIT_FAILED_PRELOAD_NO_FILE;
        LOG4CXX_ERROR(logger_, "Can't read Preload policy table file "
            << policy_config_.getPreloadPTFileName());
      }
    }
  } else {
    init_result = InitResult::INIT_OK;
    LOG4CXX_WARN(logger_, "Policy table is already created.");
  }
  return init_result;
}

//---------------------------------------------------------------

policies_ns::CheckPermissionResult
  policies_ns::PolicyManagerImpl::checkPermission(
    uint32_t app_id, const so_ns::SmartObject& rpc,
    mobile_apis::HMILevel::eType hmi_status) {

  CheckPermissionResult result =
    {PermissionResult::PERMISSION_NOK_DISALLOWED, Priority::PRIORITY_NONE};
  PolicyTable* policy_table = getPolicyTable();

  if (0 != policy_table
      && PTValidationResult::VALIDATION_OK == policy_table->Validate()) {
    so_ns::SmartObject pt_object = policy_table->AsSmartObject();

    result.result =  PermissionsCalculator::CalcPermissions(pt_object,
                                                            app_id,
                                                            rpc,
                                                            hmi_status);

    result.priority = PermissionsCalculator::GetPriority(pt_object, app_id);
  } else {
    result.result = PermissionResult::PERMISSION_NOK_PT_VERIFICATION_FAILED;
  }

  return result;
}

//---------------------------------------------------------------

void policies_ns::PolicyManagerImpl::StorePolicyTable() {
  if (0 != policy_table_) {
    if (so_ns::SmartType_Null != policy_table_->AsSmartObject().getType()) {
      const std::string pt_string = policy_table_->AsString();
      const std::vector<unsigned char> char_vector_pdata(
          pt_string.begin(), pt_string.end());
      if (false == file_system::Write(
          policy_config_.getPTFileName(), char_vector_pdata)) {
        LOG4CXX_ERROR(logger_, "Can't write policy table file "
            << policy_config_.getPTFileName());
      }
    }
  }
}

//---------------------------------------------------------------

policies_ns::PolicyTable*
  policies_ns::PolicyManagerImpl::getPolicyTable() const {
    return policy_table_;
}
