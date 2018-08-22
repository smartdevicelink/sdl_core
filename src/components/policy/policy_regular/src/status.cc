/*
 Copyright (c) 2016, Ford Motor Company
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

#include "policy/status.h"
#include "policy/update_status_manager_interface.h"

policy::UpToDateStatus::UpToDateStatus()
    : Status("UP_TO_DATE", policy::PolicyTableStatus::StatusUpToDate) {}

void policy::UpToDateStatus::ProcessEvent(UpdateStatusManagerInterface* manager,
                                          policy::UpdateEvent event) {
  switch (event) {
    case kOnNewAppRegistered:
    case kOnResetPolicyTableRequireUpdate:
    case kScheduleUpdate:
    case kOnResetRetrySequence:
      manager->SetNextStatus(std::make_shared<UpdateNeededStatus>());
      break;
    default:
      break;
  }
}

policy::UpdateNeededStatus::UpdateNeededStatus()
    : Status("UPDATE_NEEDED", policy::PolicyTableStatus::StatusUpdateRequired) {
}

void policy::UpdateNeededStatus::ProcessEvent(
    policy::UpdateStatusManagerInterface* manager, policy::UpdateEvent event) {
  switch (event) {
    case kOnUpdateSentOut:
      manager->SetNextStatus(std::make_shared<UpdatingStatus>());
      break;
    case kOnResetPolicyTableRequireUpdate:
      manager->SetNextStatus(std::make_shared<UpToDateStatus>());
      manager->SetPostponedStatus(std::make_shared<UpdateNeededStatus>());
      break;
    case kOnResetPolicyTableNoUpdate:
      manager->SetNextStatus(std::make_shared<UpToDateStatus>());
      break;
    case kOnNewAppRegistered:
      manager->SetNextStatus(std::make_shared<UpdateNeededStatus>());
      break;
    default:
      break;
  }
}

bool policy::UpdateNeededStatus::IsUpdateRequired() const {
  return true;
}

policy::UpdatingStatus::UpdatingStatus()
    : Status("UPDATING", policy::PolicyTableStatus::StatusUpdatePending) {}

void policy::UpdatingStatus::ProcessEvent(
    policy::UpdateStatusManagerInterface* manager, policy::UpdateEvent event) {
  switch (event) {
    case kOnValidUpdateReceived:
    case kOnResetPolicyTableNoUpdate:
      manager->SetNextStatus(std::make_shared<UpToDateStatus>());
      break;
    case kOnNewAppRegistered:
      manager->SetPostponedStatus(std::make_shared<UpdateNeededStatus>());
      break;
    case kOnWrongUpdateReceived:
    case kOnUpdateTimeout:
      manager->SetNextStatus(std::make_shared<UpdateNeededStatus>());
      break;
    case kOnResetPolicyTableRequireUpdate:
      manager->SetNextStatus(std::make_shared<UpToDateStatus>());
      manager->SetPostponedStatus(std::make_shared<UpdateNeededStatus>());
      break;
    case kScheduleUpdate:
    case kOnResetRetrySequence:
      manager->SetPostponedStatus(std::make_shared<UpdateNeededStatus>());
      break;
    default:
      break;
  }
}

bool policy::UpdatingStatus::IsUpdatePending() const {
  return true;
}

bool policy::UpdatingStatus::IsUpdateRequired() const {
  return true;
}

policy::Status::Status(const std::string& string_status,
                       const policy::PolicyTableStatus enum_status)
    : string_status_(string_status), enum_status_(enum_status) {}

policy::Status::~Status() {}

const std::string policy::Status::get_status_string() const {
  return string_status_;
}

policy::PolicyTableStatus policy::Status::get_status() const {
  return enum_status_;
}

bool policy::Status::IsUpdateRequired() const {
  return false;
}

bool policy::Status::IsUpdatePending() const {
  return false;
}
