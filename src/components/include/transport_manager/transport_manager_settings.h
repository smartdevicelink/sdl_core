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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_SETTINGS_H_

#include "transport_manager/transport_manager_mme_settings.h"
namespace transport_manager {
class TransportManagerSettings : public TransportManagerMMESettings {
 public:
  /**
   * @brief Returns true if last state singleton is used
   */
  virtual bool use_last_state() const = 0;

  /**
   * @brief Timeout in transport manager before disconnect
   */
  virtual uint32_t transport_manager_disconnect_timeout() const = 0;

  /**
   * @brief Returns port for TCP transport adapter
   */
  virtual uint16_t transport_manager_tcp_adapter_port() const = 0;

  /**
   * @brief Returns the millisecond count before timeout
   * for transport change feature occures.
   */
  virtual uint32_t app_transport_change_timer() const = 0;

  /**
   * @brief Returns the millisecond count as addition to
   * the transport change timeout value.
   */
  virtual uint32_t app_transport_change_timer_addition() const = 0;

  /**
   * @brief Returns the network interface name for TCP transport adapter
   */
  virtual const std::string& transport_manager_tcp_adapter_network_interface()
      const = 0;
};
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_SETTINGS_H_
