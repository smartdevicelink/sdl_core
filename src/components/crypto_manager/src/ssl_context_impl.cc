#include "crypto_manager/crypto_manager_impl.h"

#include <iostream>

namespace crypto_manager {

CryptoManagerImpl::SSLContextImpl::SSLContextImpl(SSL *conn,
                                                  BIO *bioIn,
                                                  BIO *bioOut)
  : connection_(conn),
    bioIn_(bioIn),
    bioOut_(bioOut) {
}

int CryptoManagerImpl::SSLContextImpl
::DoHandshake(char *in_data,  size_t in_data_size,
              char *out_data, size_t *out_data_size) {

  if (SSL_is_init_finished(connection_)) {
    out_data_size = 0;
    return 1;
  }



  if (in_data && (in_data_size > 0)) {
    int ret = 0;
    int err = 0;
    //int ret = BIO_write(bioIn_, in_data, in_data_size);
    //int err = SSL_get_error(connection_, ret);
    ret = SSL_write(connection_, in_data, in_data_size);
    err = SSL_get_error(connection_, ret);

    if (SSL_want_read(connection_)) {
      int pen = BIO_pending(bioOut_);
      pen = BIO_pending(bioIn_);
      if (pen) {
        ret = BIO_read(bioOut_, out_data, *out_data_size);
        if (ret > 0) {
          *out_data_size = ret;
          return ret;
        } else {
          err = SSL_get_error(connection_, ret);
          return ret;
        }
      }
      //ret = SSL_read(connection_, out_data, *out_data_size);

      //std::cout << readBytes;
    }

  }

  // int retval = SSL_do_handshake(connection_);
  return -1;
}

int CryptoManagerImpl::SSLContextImpl
::Encrypt(char *in_data,  size_t in_data_size,
          char *out_data, size_t *out_data_size) {
  return 0;
}

int CryptoManagerImpl::SSLContextImpl
::Decrypt(char *in_data,  size_t in_data_size,
          char *out_data, size_t *out_data_size) {
  return 0;
}

}  // namespace crypto_manager
