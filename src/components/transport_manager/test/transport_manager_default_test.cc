/*
 * Copyright (c) 2016, Ford Motor Company
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
#include "gtest/gtest.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_default.h"
#include "resumption/last_state.h"
#include "transport_manager/mock_transport_manager_settings.h"
#include "resumption/last_state.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::Return;

TEST(TestTransportManagerDefault, Init_LastStateNotUsed) {
  MockTransportManagerSettings transport_manager_settings;
  transport_manager::TransportManagerDefault transport_manager(
      transport_manager_settings);
  resumption::LastState last_state("app_storage_folder", "app_info_storage2");

  EXPECT_CALL(transport_manager_settings, use_last_state())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(transport_manager_settings, transport_manager_tcp_adapter_port())
      .WillRepeatedly(Return(1u));
  transport_manager.Init(last_state);
}

TEST(TestTransportManagerDefault, DISABLED_Init_LastStateUsed) {
  // TODO (dcherniev) : investigate and fix SegFault issue
  MockTransportManagerSettings transport_manager_settings;
  transport_manager::TransportManagerDefault transport_manager(
      transport_manager_settings);
  resumption::LastState last_state("app_storage_folder", "app_info_storage2");

  EXPECT_CALL(transport_manager_settings, use_last_state())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(transport_manager_settings, transport_manager_tcp_adapter_port())
      .WillRepeatedly(Return(1u));

  transport_manager.Init(last_state);
  transport_manager.Stop();
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test
