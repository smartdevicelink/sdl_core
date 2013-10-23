/**
 * @file policy_manager.cc.cc
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

namespace policies_ns = NsSmartDeviceLink::policies;
namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;

//---------------------------------------------------------------

policies_ns::PolicyManager::PolicyManager() {
}

//---------------------------------------------------------------

void policies_ns::PolicyManager::Init() {
  // TODO(anybody): read file with policy table
  // TODO()anybody): convert file content to smart object
}

//---------------------------------------------------------------

policies_ns::CheckPermissionResult::eType
  policies_ns::PolicyManager::checkPermission(
    uint32_t app_id,
    const smart_objects_ns::SmartObject& rpc) {

  return policies_ns::CheckPermissionResult::PERMISSION_OK;
}

//---------------------------------------------------------------

smart_objects_ns::CSmartSchema policies_ns::PolicyManager::createSchemaSDL() {
  return
    smart_objects_ns::CSmartSchema(
        smart_objects_ns::CAlwaysTrueSchemaItem::create());
}

//---------------------------------------------------------------

bool policies_ns::PolicyManager::validatePT(
    const smart_objects_ns::SmartObject& policy_table) {
  return true;
}
