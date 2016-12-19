#include "net/connected_socket.h"

namespace net {

ConnectedSocket::ConnectedSocket(Domain domain, const char* address, UInt32 port)
  : Socket(domain, address, port) {
}

ConnectedSocket::ConnectedSocket(Domain domain, Int32 handle)
  : Socket(domain, handle) {
}

ConnectedSocket::~ConnectedSocket() {
  shutdown();
  close();
}

bool ConnectedSocket::set_opt(Int32 level, Int32 optname,
                     const void* optval, socklen_t opt_len) {
  return Socket::set_opt(level, optname, optval, opt_len);
}

void ConnectedSocket::close() {
  Socket::close();
}

void ConnectedSocket::shutdown() {
  Socket::shutdown();
}

void ConnectedSocket::set_blocking_mode(bool is_blocking) {
  Socket::set_blocking_mode(is_blocking);
}

}  // namespace net
