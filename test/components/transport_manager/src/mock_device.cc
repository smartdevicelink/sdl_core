/*
 * \file mock_device.cc
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

#include "transport_manager/mock_device.h"

namespace test {
namespace components {
namespace transport_manager {

const ApplicationHandle MockDevice::addApplication() {
  MockApplication app(this, applications_cnt_++);
  app.device = this;
  app.active = false;
  applications_.push_back(app);
  return app.handle;
}

void MockDevice::Start() {
  for (std::vector<MockApplication>::iterator it = applications_.begin();
      it != applications_.end();
      ++it) {
    it->Start();
  }
}

void MockDevice::Stop() {
  for (std::vector<MockApplication>::iterator it = applications_.begin();
      it != applications_.end();
      ++it) {
    it->Stop();
  }
}

bool MockDevice::IsSameAs(const Device* other) const {
  return unique_device_id() == other->unique_device_id();
}

static ApplicationHandle get_handle(const MockApplication& app) {
  return app.handle;
}

ApplicationList MockDevice::GetApplicationList() const {
  ApplicationList rc(applications_.size());
  std::transform(
      applications_.begin(), applications_.end(), rc.begin(),
      &get_handle);
  return rc;
}

bool MockDevice::operator ==(const MockDevice& other) {
  return IsSameAs(&other);
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test
