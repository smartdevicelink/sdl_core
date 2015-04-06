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

#include "policy/zone_controller_impl.h"

#include <algorithm>
#include "policy/cache_manager.h"
#include "types.h"

using policy_table::DeviceData;

namespace policy {

struct IsPrimary {
  bool operator ()(const DeviceData::MapType::value_type& item) {
    return *item.second.primary == true;
  }
};

struct SetPrimary {
 private:
  bool value_;
 public:
  explicit SetPrimary(bool value) : value_(value) {
  }
  void operator ()(DeviceData::MapType::value_type& item) {
    *item.second.primary = value_;
  }
};

ZoneControllerImpl::ZoneControllerImpl(CacheManager& cache)
    : cache_(cache),
      access_(kManual),
      primary_device_() {
  DeviceData& devices = *cache_.pt_->policy_table.device_data;
  DeviceData::iterator d = std::find_if(devices.begin(), devices.end(),
                                        IsPrimary());
  if (d != devices.end()) {
    primary_device_ = d->first;
  }
}

bool ZoneControllerImpl::IsPrimaryDevice(const PTString& dev_id) const {
  return primary_device_ == dev_id;
}

bool ZoneControllerImpl::IsPassengerZone(const SeatLocation& seat,
                                         const SeatLocation& zone) const {
  return seat == zone;
}

TypeAccess ZoneControllerImpl::CheckAccess(const PTString& dev_id,
                                           const PTString& app_id,
                                           const PTString& func_id,
                                           const SeatLocation& zone) const {
  return access_;
}

void ZoneControllerImpl::AddAccess(const PTString& dev_id,
                                   const PTString& app_id,
                                   const PTString& func_id) {
  access_ = kAllowed;
  cache_.Backup();
}

void ZoneControllerImpl::RemoveAccess(const PTString& func_id) {
  access_ = kManual;
  cache_.Backup();
}

void ZoneControllerImpl::SetDriverDevice(const PTString& dev_id) {
  primary_device_ = dev_id;
  DeviceData& devices = *cache_.pt_->policy_table.device_data;
  std::for_each(devices.begin(), devices.end(), SetPrimary(false));
  *devices[dev_id].primary = true;
  cache_.Backup();
}

}  // namespace policy
