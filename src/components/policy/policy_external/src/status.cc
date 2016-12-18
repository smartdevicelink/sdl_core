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
#include "policy/update_status_manager.h"
#include "utils/make_shared.h"

void policy::UpToDateStatus::ProcessEvent(UpdateStatusManager* mng,
                                          policy::UpdateEvent event) {
  switch (event) {
    case kOnNewAppRegistered:
    case kOnResetPolicyTableRequireUpdate:
    case kScheduleUpdate:
    case kOnResetRetrySequence:
      mng->SetNextStatus(new UpdateNeededStatus());
      break;
    default:
      break;
  }
}

const std::string policy::UpToDateStatus::get_status_string() const {
  return "UP_TO_DATE";
}

policy::PolicyTableStatus policy::UpToDateStatus::get_status() const {
  return PolicyTableStatus::StatusUpToDate;
}

void policy::UpdateNeededStatus::ProcessEvent(policy::UpdateStatusManager* mng,
                                              policy::UpdateEvent event) {
  switch (event) {
    case kOnUpdateSentOut:
      mng->SetNextStatus(utils::MakeShared<UpdatingStatus>());
      break;
    case kOnResetPolicyTableRequireUpdate:
      mng->SetNextStatus(utils::MakeShared<UpToDateStatus>());
      mng->SetPostponedStatus(utils::MakeShared<UpdateNeededStatus>());
      break;
    case kOnResetPolicyTableNoUpdate:
      mng->SetNextStatus(utils::MakeShared<UpToDateStatus>());
      break;
    default:
      break;
  }
}

const std::string policy::UpdateNeededStatus::get_status_string() const {
  return "UPDATE_NEEDED";
}

policy::PolicyTableStatus policy::UpdateNeededStatus::get_status() const {
  return PolicyTableStatus::StatusUpdateRequired;
}

bool policy::UpdateNeededStatus::IsUpdateRequired() const {
  return true;
}

void policy::UpdatingStatus::ProcessEvent(policy::UpdateStatusManager* mng,
                                          policy::UpdateEvent event) {
  switch (event) {
    case kOnValidUpdateReceived:
    case kOnResetPolicyTableNoUpdate:
      mng->SetNextStatus(utils::MakeShared<UpToDateStatus>());
      break;
    case kOnNewAppRegistered:
      mng->SetPostponedStatus(utils::MakeShared<UpdateNeededStatus>());
      break;
    case kOnWrongUpdateReceived:
    case kOnUpdateTimeout:
      mng->SetNextStatus(utils::MakeShared<UpdateNeededStatus>());
      break;
    case kOnResetPolicyTableRequireUpdate:
      mng->SetNextStatus(utils::MakeShared<UpToDateStatus>());
      mng->SetPostponedStatus(utils::MakeShared<UpdateNeededStatus>());
      break;
    case kScheduleUpdate:
    case kOnResetRetrySequence:
      mng->SetPostponedStatus(utils::MakeShared<UpdateNeededStatus>());
      break;
    default:
      break;
  }
}

const std::string policy::UpdatingStatus::get_status_string() const {
  return "UPDATING";
}

policy::PolicyTableStatus policy::UpdatingStatus::get_status() const {
  return PolicyTableStatus::StatusUpdatePending;
}

bool policy::UpdatingStatus::IsUpdatePending() const {
  return true;
}

bool policy::UpdatingStatus::IsUpdateRequired() const {
  return true;
}

policy::Status::~Status() {}

bool policy::Status::IsUpdateRequired() const {
  return false;
}

bool policy::Status::IsUpdatePending() const {
  return false;
}
