#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include "crypto_manager/crypto_manager.h"
#include "crypto_manager/crypto_manager_impl.h"
#include "crypto_manager/ssl_context.h"

using namespace crypto_manager;

int main() {
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();
  SSL_library_init();

  SSL_CTX * ctx = SSL_CTX_new(SSLv23_client_method());

  SSL * connection = SSL_new(ctx);

  BIO* bioIn = BIO_new(BIO_s_mem());
  BIO* bioOut = BIO_new(BIO_s_mem());
  SSL_set_bio(connection, bioIn, bioOut);

  SSL_set_connect_state(connection);


  CryptoManager* crypto_manager = new CryptoManagerImpl();
  crypto_manager->Init();

  SSLContext *server_ctx = crypto_manager->CreateSSLContext();

  int res = 0;

  char *outBuf = new char[1024 * 1024];
  char *inBuf = new char[1024 * 1024];

  for(;;) {
    res = SSL_do_handshake(connection);
    if (res < 0) {
      int error = SSL_get_error(connection, res);
      // break if fatal error
      if (error != SSL_ERROR_WANT_READ &&
          error != SSL_ERROR_WANT_WRITE) {
        break;
      }
      size_t outLen;
      size_t inLen = 1024 * 1024;
      outLen = BIO_ctrl_pending(bioOut);
      if (outLen) {
        BIO_read(bioOut, outBuf, outLen);
      }
      int shret = server_ctx->DoHandshake(outBuf, outLen, inBuf, &inLen);

      if (inLen) {
        BIO_write(bioIn, inBuf, inLen);
      }
      if (shret == 0) {
        res = SSL_do_handshake(connection);
        break;
      }
    }
  }

  EVP_cleanup();
}
