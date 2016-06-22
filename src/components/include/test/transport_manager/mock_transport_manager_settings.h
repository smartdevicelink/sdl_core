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
#ifndef SRC_COMPONENTS_INCLUDE_TEST_TRANSPORT_MANAGER_MOCK_TRANSPORT_MANAGER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_TEST_TRANSPORT_MANAGER_MOCK_TRANSPORT_MANAGER_SETTINGS_H_

#include "gmock/gmock.h"
#include "protocol_handler/protocol_handler.h"
#include "transport_manager/transport_manager_settings.h"
#include "mock_transport_manager_mme_settings.h"

namespace test {
namespace components {
namespace transport_manager_test {

class MockTransportManagerSettings
    : public ::transport_manager::TransportManagerSettings {
 public:
  MOCK_CONST_METHOD0(use_last_state, bool());
  MOCK_CONST_METHOD0(transport_manager_disconnect_timeout, uint32_t());
  MOCK_CONST_METHOD0(transport_manager_tcp_adapter_port, uint16_t());

  // from mme settings
  MOCK_CONST_METHOD0(event_mq_name, const std::string&());
  MOCK_CONST_METHOD0(ack_mq_name, const std::string&());
  MOCK_CONST_METHOD0(iap2_hub_connect_attempts, uint32_t());
  MOCK_CONST_METHOD0(default_hub_protocol_index, uint32_t());
  MOCK_CONST_METHOD0(iap_legacy_protocol_mask, const std::string&());
  MOCK_CONST_METHOD0(iap_hub_protocol_mask, const std::string&());
  MOCK_CONST_METHOD0(iap_pool_protocol_mask, const std::string&());
  MOCK_CONST_METHOD0(iap_system_config, const std::string&());
  MOCK_CONST_METHOD0(iap2_system_config, const std::string&());
  MOCK_CONST_METHOD0(iap_hub_connection_wait_timeout, uint32_t());
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_TRANSPORT_MANAGER_MOCK_TRANSPORT_MANAGER_SETTINGS_H_
