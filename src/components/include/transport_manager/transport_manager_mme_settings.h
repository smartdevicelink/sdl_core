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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_MME_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_MME_SETTINGS_H_

#include <stdint.h>
#include <string>
#include <vector>

namespace transport_manager {
class TransportManagerMMESettings {
 public:
  virtual const std::string& event_mq_name() const = 0;

  virtual const std::string& ack_mq_name() const = 0;

  virtual uint32_t iap2_hub_connect_attempts() const = 0;

  virtual uint32_t default_hub_protocol_index() const = 0;

  virtual const std::string& iap_legacy_protocol_mask() const = 0;

  virtual const std::string& iap_hub_protocol_mask() const = 0;

  virtual const std::string& iap_pool_protocol_mask() const = 0;

  virtual const std::string& iap_system_config() const = 0;

  virtual const std::string& iap2_system_config() const = 0;

  virtual uint32_t iap_hub_connection_wait_timeout() const = 0;
};
}

#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_MME_SETTINGS_H_
