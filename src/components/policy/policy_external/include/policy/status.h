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

#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_STATUS_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_STATUS_H_

#include <string>
#include "policy/policy_types.h"
#include "utils/macro.h"

namespace policy {

/**
 * @brief The UpdateEvent enum defines system events which can change update
 * status
 */
enum UpdateEvent {
  kOnNewAppRegistered,
  kOnValidUpdateReceived,
  kOnWrongUpdateReceived,
  kOnUpdateSentOut,
  kOnUpdateTimeout,
  kOnResetPolicyTableRequireUpdate,
  kOnResetPolicyTableNoUpdate,
  kScheduleUpdate,
  kScheduleManualUpdate,
  kOnResetRetrySequence,
  kNoEvent
};

const std::string kUpToDate = "UP_TO_DATE";
const std::string kUpdateNeeded = "UPDATE_NEEDED";
const std::string kUpdating = "UPDATING";

class UpdateStatusManager;

/**
 * @brief The Status class defines base implementation of update status
 */
class Status {
 public:
  /**
   * @brief Constructor
   */
  Status(const std::string& string_status,
         const policy::PolicyTableStatus enum_status);

  /**
 * @brief Destructor
 */
  virtual ~Status();

  /**
 * @brief Process event by setting next status in case event can affect
 * current status or ignores the event
 * @param manager Status manager pointer
 * @param event Event which needs to be processed
 */
  virtual void ProcessEvent(UpdateStatusManager* manager,
                            UpdateEvent event) = 0;

  /**
 * @brief Return current status as string value
 * @return Status as string
 */
  const std::string get_status_string() const;

  /**
 * @brief Return status as enum value
 * @return Status as enum value
 */
  PolicyTableStatus get_status() const;

  /**
 * @brief Check whether update is required in terms of status
 * @return True if update is required, otherwise - false
 */
  virtual bool IsUpdateRequired() const;

  /**
 * @brief Check whether update is pending in terms of status
 * @return True if update is pending, otherwise - false
 */
  virtual bool IsUpdatePending() const;

 private:
  const std::string string_status_;
  const PolicyTableStatus enum_status_;
};

/**
 * @brief The UpToDateStatus class represents 'up-to-date' status
 */
class UpToDateStatus : public Status {
 public:
  /**
   * @brief Constructor
   */
  UpToDateStatus();

  /**
 * @brief Process event by setting next status in case event can affect
 * current status or ignores the event
 * @param manager Status manager pointer
 * @param event Event which needs to be processed
 */
  void ProcessEvent(UpdateStatusManager* manager, UpdateEvent event) OVERRIDE;
};

/**
 * @brief The UpdateNeededStatus class represents 'update needed' status
 */
class UpdateNeededStatus : public Status {
 public:
  /**
   * @brief Constructor
   */
  UpdateNeededStatus();

  /**
 * @brief Process event by setting next status in case event can affect
 * current status or ignores the event
 * @param manager Status manager pointer
 * @param event Event which needs to be processed
 */
  void ProcessEvent(UpdateStatusManager* manager, UpdateEvent event) OVERRIDE;

  /**
 * @brief Check whether update is required in terms of status
 * @return True if update is required, otherwise - false
 */
  bool IsUpdateRequired() const OVERRIDE;
};

/**
 * @brief The UpdatingStatus class represents 'updating' status
 */
class UpdatingStatus : public Status {
 public:
  /**
   * @brief Constructor
   */
  UpdatingStatus();

  /**
 * @brief Process event by setting next status in case event can affect
 * current status or ignores the event
 * @param manager Status manager pointer
 * @param event Event which needs to be processed
 */
  void ProcessEvent(UpdateStatusManager* manager, UpdateEvent event) OVERRIDE;

  /**
 * @brief Check whether update is required in terms of status
 * @return True if update is required, otherwise - false
 */
  bool IsUpdateRequired() const OVERRIDE;

  /**
 * @brief Check whether update is pending in terms of status
 * @return True if update is pending, otherwise - false
 */
  bool IsUpdatePending() const OVERRIDE;
};
}

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_STATUS_H_
