
#ifndef SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_CRYPTO_MANAGER_SETTINGS_IMPL_H_
#define SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_CRYPTO_MANAGER_SETTINGS_IMPL_H_
#include "security_manager/security_manager_settings.h"
#include "config_profile/profile.h"

namespace security_manager {

class CryptoManagerSettingsImpl : public CryptoManagerSettings {
 public:
  CryptoManagerSettingsImpl(const profile::Profile& profile,
                            const std::string& certificate_data)
      : profile_(profile), certificate_data_(certificate_data) {}

  // CryptoManagerSettings interface
  Mode security_manager_mode() const OVERRIDE {
    return profile_.ssl_mode() == "SERVER" ? security_manager::SERVER
                                           : security_manager::CLIENT;
  }
  Protocol security_manager_protocol_name() const OVERRIDE {
    CREATE_LOGGERPTR_LOCAL(logger_, "SecurityManager")

    const std::string& protocol_str = profile_.security_manager_protocol_name();
    if (protocol_str == "TLSv1.0") {
      return security_manager::TLSv1;
    }
    if (protocol_str == "TLSv1.1") {
      return security_manager::TLSv1_1;
    }
    if (protocol_str == "TLSv1.2") {
      return security_manager::TLSv1_2;
    }
    if (protocol_str == "SSLv3") {
      return security_manager::SSLv3;
    }
    LOG4CXX_ERROR(
          logger_,
          "Unknown protocol: " << profile::Profile::instance()
                                      ->security_manager_protocol_name());
    return static_cast<security_manager::Protocol>(-1);
  }
  bool verify_peer() const OVERRIDE {
    return profile_.verify_peer();
  }
  const std::string& certificate_data() const OVERRIDE {
    return certificate_data_;
  }
  const std::string& ciphers_list() const OVERRIDE {
    return profile_.ciphers_list();
  }
  const std::string& ca_cert_path() const OVERRIDE {
    return profile_.ca_cert_path();
  }
  size_t update_before_hours() const OVERRIDE {
    return profile_.update_before_hours();
  }
  size_t maximum_payload_size() const OVERRIDE {
    return profile_.maximum_payload_size();
  }

 private:
  const profile::Profile& profile_;
  const std::string certificate_data_;
};
}
#endif  // SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_CRYPTO_MANAGER_SETTINGS_IMPL_H_
