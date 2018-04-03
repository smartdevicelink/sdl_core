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

#ifndef SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_CRYPTO_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_CRYPTO_MANAGER_H_

#include <time.h>
#include "application_manager/policies/policy_handler_observer.h"
#include "security_manager/security_manager_settings.h"

/**
 * \class security_manager::CryptoManager
 * \brief Class factory, producing instances of \ref SSLContext
 *
 * \fn security_manager::SSLContext
 **security_manager::CryptoManager::CreateSSLContext()
 * \brief Creates an instance of \ref SSLContext class
 *
  * \fn void
 *security_manager::CryptoManager::ReleaseSSLContext(security_manager::SSLContext
 **context)
 * \brief Frees \ref SSLContext instance
 */

namespace security_manager {
class SSLContext;

class CryptoManager : public policy::PolicyHandlerObserver {
 public:
  /**
   * @brief Init allows to initialize cryptomanager with certain values.
   *
   * @return true in case initialization was succesful, false otherwise.
   */
  virtual bool Init() = 0;
  virtual SSLContext* CreateSSLContext() = 0;
  virtual bool OnCertificateUpdated(const std::string& data) = 0;
  virtual void ReleaseSSLContext(SSLContext* context) = 0;
  virtual std::string LastError() const = 0;
  /**
  * @brief IsCertificateUpdateRequired checks if certificate update is needed
  * @param system_time - time with which certificate expiration time
  * should be compared
  * @param certificates_time - certificate expiration time
  * @return True if certificate expired and need to be updated
  * otherwise False
  */
  virtual bool IsCertificateUpdateRequired(
      const time_t system_time, const time_t certificates_time) const = 0;
  /**
  * \brief Crypto manager settings getter
  * \return pointer to crypto manager settings class
  */
  virtual const CryptoManagerSettings& get_settings() const = 0;
  virtual ~CryptoManager() {}
};

}  // namespace security_manager
#endif  // SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_CRYPTO_MANAGER_H_
