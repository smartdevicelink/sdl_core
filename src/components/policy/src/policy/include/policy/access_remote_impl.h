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
#ifndef SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_IMPL_H_
#define SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_IMPL_H_

#include <map>
#include "types.h"
#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "policy/access_remote.h"
#include "policy/cache_manager.h"

using policy_table::FunctionalGroupings;

namespace policy {

struct Match {
 private:
  const FunctionalGroupings& groups_;
  const PTString& rpc_;
  const RemoteControlParams& params_;
 public:
  Match(const FunctionalGroupings& groups, const PTString& rpc,
        const RemoteControlParams& params);
  bool operator ()(const PTString& item) const;
};

class AccessRemoteImpl : public AccessRemote {
 public:
  AccessRemoteImpl();
  explicit AccessRemoteImpl(utils::SharedPtr<CacheManager> cache);

  virtual void Init();
  virtual void Enable();
  virtual void Disable();
  virtual bool IsEnabled() const;

  virtual bool IsPrimaryDevice(const PTString& dev_id) const;
  virtual void SetPrimaryDevice(const PTString& dev_id);
  virtual bool IsPassengerZone(const SeatLocation& seat,
                               const SeatLocation& zone) const;

  virtual void Allow(const Subject& who, const Object& what);
  virtual void Deny(const Subject& who, const Object& what);
  virtual void Reset(const Subject& who);
  virtual void Reset(const Object& what);
  virtual TypeAccess Check(const Subject& who, const Object& what) const;
  virtual PTString FindGroup(const Subject& who, const PTString& rpc,
                             const RemoteControlParams& params) const;
  virtual void SetDefaultHmiTypes(const std::string& app_id,
                                  const std::vector<int>& hmi_types);
  virtual const policy_table::Strings& GetGroups(const PTString& device_id,
                                                 const PTString& app_id);

 private:
  typedef std::map<Subject, TypeAccess> AccessControlRow;
  typedef std::map<Object, AccessControlRow> AccessControlList;
  typedef std::map<std::string, policy_table::AppHMITypes> HMIList;
  inline void set_enabled(bool value);
  const policy_table::AppHMITypes& HmiTypes(const std::string& app_id);

  utils::SharedPtr<CacheManager> cache_;
  PTString primary_device_;
  bool enabled_;
  bool country_consent_;
  AccessControlList acl_;
  HMIList hmi_types_;

  friend struct Erase;
  friend struct IsTypeAccess;

  FRIEND_TEST(AccessRemoteImplTest, KeyMapTest);
  FRIEND_TEST(AccessRemoteImplTest, Allow);
  FRIEND_TEST(AccessRemoteImplTest, Deny);
  FRIEND_TEST(AccessRemoteImplTest, ChangeAccess);
  FRIEND_TEST(AccessRemoteImplTest, ResetBySubject);
  FRIEND_TEST(AccessRemoteImplTest, ResetByObject);
  FRIEND_TEST(AccessRemoteImplTest, CheckAllowed);
  FRIEND_TEST(AccessRemoteImplTest, CheckDisallowed);
  FRIEND_TEST(AccessRemoteImplTest, CheckManual);
  FRIEND_TEST(AccessRemoteImplTest, SetPrimaryDevice);
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_ACCESS_REMOTE_IMPL_H_
