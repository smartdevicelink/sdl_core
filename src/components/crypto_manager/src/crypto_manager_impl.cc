#include "crypto_manager/crypto_manager_impl.h"

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace crypto_manager {

bool CryptoManagerImpl::Init() {
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
  SSL_library_init();

  context_ = SSL_CTX_new(SSLv23_server_method());
  SSL_CTX_set_verify(context_, SSL_VERIFY_NONE, NULL);
  //SSL_CTX_set_options(context_, SSL_OP_NO_SSLv2);

  return true;
}

SSLContext * CryptoManagerImpl::CreateSSLContext() {
  SSL* conn = SSL_new(context_);
  if (conn == NULL)
    return NULL;

  BIO *bioIn = BIO_new(BIO_s_mem());
  BIO *bioOut = BIO_new(BIO_s_mem());
  SSL_set_bio(conn, bioIn, bioOut);

  SSL_set_accept_state(conn);
  SSL_accept(conn);
  return new SSLContextImpl(conn, bioIn, bioOut);
}

} // namespace crypto_manager
