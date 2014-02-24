#ifndef SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_CRYPTO_MANAGER_H
#define SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_CRYPTO_MANAGER_H

namespace crypto_manager {
class SSLContext;
class CryptoManager {
 public:
  virtual bool Init()=0;
  virtual SSLContext *CreateSSLContext()=0;
  virtual ~CryptoManager() { }
};

} // namespace crypto_manager

#endif // SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_CRYPTO_MANAGER_H
