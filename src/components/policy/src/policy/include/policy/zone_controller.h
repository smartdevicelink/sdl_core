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
#ifndef SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ZONE_CONTROLLER_H_
#define SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ZONE_CONTROLLER_H_

#include "policy/policy_types.h"

namespace policy {

enum TypeAccess {
  kDisallowed,
  kAllowed,
  kManual
};

typedef int SeatLocation;

class ZoneController {
 public:
  virtual ~ZoneController() {
  }

  /**
   * Checks whether device is driver's device
   * @param dev_id unique device id
   * @return true if device is have driver
   */
  virtual bool IsPrimaryDevice(const PTString& dev_id) const = 0;

  /**
   * Checks passenger can control of the requested zone without asking driver
   * @param seat seat of passenger
   * @param zone requested zone to control
   * @return true if passenger can control this zone
   */
  virtual bool IsPassengerZone(const SeatLocation& seat,
                               const SeatLocation& zone) const = 0;

  /**
   * Checks user has access to control equipment
   * @param dev_id unique device id
   * @param app_id application id
   * @param func_id name of RPC
   * @param zone requested zone to control
   * @return allowed if access was given, disallowed if access was denied
   * manual if need to ask driver
   */
  virtual policy::TypeAccess CheckAccess(const PTString& dev_id,
                                         const PTString& app_id,
                                         const PTString& func_id,
                                         const SeatLocation& zone) const = 0;

  /**
   * Sets access to control equipment
   * @param dev_id unique device id
   * @param app_id application id
   * @param func_id name of RPC
   */
  virtual void AddAccess(const PTString& dev_id, const PTString& app_id,
                         const PTString& func_id) = 0;

  /**
   * Resets access to control equipment for all passengers' devices by name of RPC
   * @param func_id name of RPC
   */
  virtual void RemoveAccess(const PTString& func_id) = 0;

  /**
   * Sets device as driver's device
   * @param dev_id ID device
   */
  virtual void SetDriverDevice(const PTString& dev_id) = 0;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ZONE_CONTROLLER_H_
