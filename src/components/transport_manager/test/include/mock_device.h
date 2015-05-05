/*
 * \file mock_device.h
 * \brief 
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

#ifndef APPLINK_TEST_COMPONENTS_TRANSPORTMANAGER_INCLUDE_MOCKDEVICE_H_
#define APPLINK_TEST_COMPONENTS_TRANSPORTMANAGER_INCLUDE_MOCKDEVICE_H_

#include "transport_manager/common.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <transport_manager/transport_adapter/transport_adapter_impl.h>
#include <transport_manager/transport_manager_impl.h>
#include "mock_device_scanner.h"
#include "mock_application.h"

using ::transport_manager::ApplicationList;
using ::transport_manager::transport_adapter::Device;
using ::transport_manager::transport_adapter::TransportAdapterController;

namespace test {
namespace components {
namespace transport_manager {

class MockDevice : public ::transport_manager::transport_adapter::Device {

  pthread_mutex_t device_started_mutex;
  std::vector<MockApplication> applications_;
  int applications_cnt_;
  TransportAdapterController *controller_;
 public:
  MockDevice(const std::string& name, const std::string& id,
             TransportAdapterController * controller)
      : Device(name, id),
        applications_cnt_(0),
        controller_(controller) {
  }
  const ApplicationHandle addApplication();
  void Start();
  void Stop();
  bool IsSameAs(const Device* other) const;
  ApplicationList GetApplicationList() const;
  bool operator == (const MockDevice &other);
};

}  // namespace transport_manager
}  // namespace components
}  // namespace test

#endif /* APPLINK_TEST_COMPONENTS_TRANSPORTMANAGER_INCLUDE_MOCKDEVICE_H_ */
