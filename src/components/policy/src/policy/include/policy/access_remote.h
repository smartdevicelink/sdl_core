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
#ifndef SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_H_
#define SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_H_

#include "policy/policy_types.h"

namespace policy {

typedef int SeatLocation;

enum TypeAccess {
  kDisallowed,
  kAllowed,
  kManual
};

struct Subject {
  PTString dev_id;
  PTString app_id;
};
inline bool operator<(const Subject& x, const Subject& y) {
  return x.dev_id < y.dev_id && x.app_id < y.app_id;
}

struct Object {
  PTString func_id;
  SeatLocation zone;
};
inline bool operator<(const Object& x, const Object& y) {
  return x.func_id < y.func_id && x.zone < y.zone;
}

class AccessRemote {
 public:
  virtual ~AccessRemote() {
  }

  /**
   * Enables remote control
   */
  virtual void Enable() = 0;

  /**
   * Disables remote control
   */
  virtual void Disable() = 0;

  /**
   * Checks if remote control is enabled
   * @return true if enabled
   */
  virtual bool IsEnabled() const = 0;

  /**
   * Checks whether device is driver's device
   * @param dev_id unique device id
   * @return true if device is have driver
   */
  virtual bool IsPrimaryDevice(const PTString& dev_id) const = 0;

  /**
   * Sets device as driver's device
   * @param dev_id ID device
   */
  virtual void SetPrimaryDevice(const PTString& dev_id) = 0;

  /**
   * Checks passenger can control of the requested zone without asking driver
   * @param seat seat of passenger
   * @param zone requested zone to control
   * @return true if passenger can control this zone
   */
  virtual bool IsPassengerZone(const SeatLocation& seat,
                               const SeatLocation& zone) const = 0;

  /**
   * Allows access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id and zone
   */
  virtual void Allow(const Subject& who, const Object& what) = 0;

  /**
   * Denies access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id and zone
   */
  virtual void Deny(const Subject& who, const Object& what) = 0;

  /**
   * Resets access subject to all object
   * @param who subject is dev_id and app_id
   */
  virtual void Reset(const Subject& who) = 0;

  /**
   * Checks access subject to object
   * @param who subject is dev_id and app_id
   * @param what object is group_id and zone
   * @return allowed if access was given, disallowed if access was denied
   * manual if need to ask driver
   */
  virtual TypeAccess Check(const Subject& who, const Object& what) const = 0;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_H_
