#ifndef SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_SSL_CONTEXT_H
#define SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_SSL_CONTEXT_H

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace crypto_manager {
class SSLContext {
 public:
  int DoHandshake(char *in_data,  size_t  in_data_size,
                  char *out_data, size_t *out_data_size);
  int Encrypt(char *in_data,  size_t  in_data_size,
              char *out_data, size_t *out_data_size);
  int Decrypt(char *in_data,  size_t  in_data_size,
              char *out_data, size_t *out_data_size);
};
} // namespace crypto_manager
#endif // SRC_COMPONENTS_CRYPTO_MANAGER_INCLUDE_CRYPTO_MANAGER_SSL_CONTEXT_H
