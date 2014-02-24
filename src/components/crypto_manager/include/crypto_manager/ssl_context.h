#ifndef SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_SSL_CONTEXT_H
#define SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_SSL_CONTEXT_H

#include <cstddef> // for size_t typedef

namespace crypto_manager {
class SSLContext {
 public:
  virtual int DoHandshake(char *in_data,  size_t  in_data_size,
                  char *out_data, size_t *out_data_size)=0;
  virtual int Encrypt(char *in_data,  size_t  in_data_size,
              char *out_data, size_t *out_data_size)=0;
  virtual int Decrypt(char *in_data,  size_t  in_data_size,
              char *out_data, size_t *out_data_size)=0;
  virtual ~SSLContext() { };
};
} // namespace crypto_manager
#endif // SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_SSL_CONTEXT_H
