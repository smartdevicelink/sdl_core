/**
 * \file server_connection_factory.h
 * \brief ServerConnectionFactory class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_SERVER_CONNECTION_FACTORY_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_SERVER_CONNECTION_FACTORY_H_

#include "transport_manager/transport_adapter/transport_adapter.h"

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Implement transport dependent connection that was originated by the user.
 */
class ServerConnectionFactory {
 public:
  /**
   * @brief Start server connection factory.
   *
   * @return Information about possible reason of error during initialization.
   */
  virtual TransportAdapter::Error Init() = 0;

  /**
   * @brief
   *
   * @param device_handle Device unique identifier.
   * @param app_handle Handle of application.
   *
   * @return Information about posible reason of error.
   */
  virtual TransportAdapter::Error CreateConnection(
      const DeviceUID& device_handle, const ApplicationHandle& app_handle) = 0;

  /**
   * @brief
   */
  virtual void Terminate() = 0;

  /**
   * @brief Check device scanner for initialization.
   *
   * @return true - initialized.
   * false - not initialized.
   */
  virtual bool IsInitialised() const = 0;

  /**
   * @brief Destructor.
   */
  virtual ~ServerConnectionFactory() {
  }
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* SERVER_CONNECTION_FACTORY_H_ */
