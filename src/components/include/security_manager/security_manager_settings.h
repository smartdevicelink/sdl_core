/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_SUCURITY_MANAGER_SUCURITY_MANAGER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_SUCURITY_MANAGER_SUCURITY_MANAGER_SETTINGS_H_

namespace security_manager {
enum Mode { CLIENT, SERVER };
enum Protocol { SSLv3, TLSv1, TLSv1_1, TLSv1_2 };
/**
 * \class ConnectionHandlerSettings
 * \brief Interface for connection handler component settings.
 */
class CryptoManagerSettings {
 public:
  virtual ~CryptoManagerSettings() {}

  virtual Mode security_manager_mode() const = 0;
  virtual Protocol security_manager_protocol_name() const = 0;
  virtual bool verify_peer() const = 0;
  virtual const std::string& certificate_data() const = 0;
  virtual const std::string& ciphers_list() const = 0;
  virtual const std::string& ca_cert_path() const = 0;
  virtual size_t update_before_hours() const = 0;
  virtual size_t maximum_payload_size() const = 0;
};

}  // namespace security_manager
#endif  // SRC_COMPONENTS_INCLUDE_SUCURITY_MANAGER_SUCURITY_MANAGER_SETTINGS_H_
