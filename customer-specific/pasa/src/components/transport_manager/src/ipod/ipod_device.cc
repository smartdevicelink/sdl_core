/**
 * \file ipod_device.cc
 * \brief ipodDevice class source file.
 *
 * Copyright (c) 2013, Ford Motor Company
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

#include "transport_manager/ipod/ipod_device.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <limits>



namespace transport_manager {
namespace transport_adapter {


IpodDevice::IpodDevice(const char* name, uint64_t srcid)
    : Device(name, std::string("IPOD-") + std::string(name))
{
    srcID = srcid;
}

const uint64_t IpodDevice::GetSrcId() const {
  return srcID;
}

bool IpodDevice::IsSameAs(const Device* other) const {
  bool result = false;

  const IpodDevice* other_ipod_device =
      dynamic_cast<const IpodDevice*>(other);

  if (0 != other_ipod_device) {
    if (0
        == (unique_device_id() != other_ipod_device->unique_device_id())) {
      result = true;
    }
  }

  return result;
}

ApplicationList IpodDevice::GetApplicationList() const {
// Todd: Multiple protocol strings support
//  return ApplicationList(rfcomm_channels_.begin(), rfcomm_channels_.end());
  return ApplicationList(1, 1);
}

}  // namespace transport_adapter
}  // namespace transport_manager

