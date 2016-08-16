#include "net/connected_socket_impl.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils/logger.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

namespace net {

ConnectedSocket* ConnectedSocketImpl::ConnectToHost(const char* address,
                                                    UInt32 port,
                                                    UInt32 attempts) {
  ConnectedSocket* socket = new ConnectedSocketImpl(address, port);
  UInt32 count = 0;
  while (!socket->connect()) {
    LOG4CXX_ERROR(logger_, "Unable to connect " << address << ":" << port);
    delete socket;
    socket = new ConnectedSocketImpl(address, port);
    sleep(1);
    if (attempts != 0 && ++count == attempts) {
      LOG4CXX_INFO(
          logger_,
          "Unable to connect " << address << ":" << port << " with " << attempts << " attempts");
      delete socket;
      return NULL;
    }
  }
  LOG4CXX_ERROR(logger_, "Connected to connect " << address << ":" << port);
  return socket;
}

ConnectedSocket* ConnectedSocketImpl::ConnectToHost(const char* path_name) {
  ConnectedSocket* socket = new ConnectedSocketImpl(path_name);
  if (!socket->connect()) {
    LOG4CXX_ERROR(logger_, "Unable to connect to " << path_name);
    delete socket;
    return NULL;
  }
  return socket;
}

ConnectedSocketImpl::ConnectedSocketImpl(const char* address, UInt32 port)
    : ConnectedSocket(NET_AF_INET, address, port) {}

ConnectedSocketImpl::ConnectedSocketImpl(const char* path_name)
    : ConnectedSocket(NET_AF_UNIX, path_name, 0) {}

ConnectedSocketImpl::ConnectedSocketImpl(Domain domain, Int32 handle)
    : ConnectedSocket(domain, handle) {}

ConnectedSocketImpl::~ConnectedSocketImpl() {}

bool ConnectedSocketImpl::set_opt(Int32 level, Int32 optname,
                                  const void* optval, socklen_t opt_len) {
  return ConnectedSocket::set_opt(level, optname, optval, opt_len);
}

void ConnectedSocketImpl::close() { ConnectedSocket::close(); }

void ConnectedSocketImpl::shutdown() { ConnectedSocket::shutdown(); }

void ConnectedSocketImpl::set_blocking_mode(bool is_blocking) {
  ConnectedSocket::set_blocking_mode(is_blocking);
}

bool ConnectedSocketImpl::connect() {
  DCHECK_OR_RETURN(socket_handle() != INVALID_SOCKET, false);
  struct sockaddr_in addr_in = {0};
  struct sockaddr_un addr_un = {0};
  struct sockaddr* addr = NULL;
  socklen_t len = 0;

  if (NET_AF_UNIX == domain()) {
    addr_un.sun_family = domain();
    memcpy(addr_un.sun_path, address(), sizeof(addr_un.sun_path));
    addr = reinterpret_cast<struct sockaddr*>(&addr_un);
    len = sizeof(addr_un);
  } else {
    addr_in.sin_addr.s_addr = inet_addr(address());
    addr_in.sin_family = domain();
    addr_in.sin_port = htons(port());
    addr = reinterpret_cast<struct sockaddr*>(&addr_in);
    len = sizeof(addr_in);
  }

  if (-1 == ::connect(socket_handle(), addr, len)) {
    LOG4CXX_ERROR(logger_, "Unable to connect to the server " << strerror(errno));
    return false;
  }

  return true;
}

ssize_t ConnectedSocketImpl::send(const UInt8* buffer, size_t size,
                                  Int32 flags) {
  ssize_t ret;
  if (-1 == (ret = ::send(socket_handle(), buffer, size, flags))) {
    LOG4CXX_ERROR(logger_, "Unable to send " << strerror(errno));
  }

  return ret;
}

ssize_t ConnectedSocketImpl::send(Int32 file_fd, Int32 flags) {
  ssize_t ret = -1;

  if (NET_AF_UNIX != domain()) {
    return ret;
  }

  /* We are passing at least one byte of data so that recvmsg() will not return
   * 0 */
  char data[1];
  data[0] = ' ';

  /* The struct iovec below is part of a scatter-gather list.  It describes a
     buffer.  In this case, it describes a buffer (an integer) containing the
     data that we're going to send back
  */
  struct iovec iov[1];
  iov[0].iov_base = data;
  iov[0].iov_len = sizeof(data);

  /* The CMSG macros yused to create and access control
     messages (which is another name for ancillary data). The ancillary
     data is made up of pairs of struct cmsghdr structures and associated
     data arrays. First, we're going to allocate a buffer on the stack to
     contain our
     data array (that contains the socket).
  */
  struct cmsghdr* cmp;
  cmp = static_cast<cmsghdr*>(malloc(CMSG_SPACE(sizeof(int))));

  /* There is a msghdr that is used to minimize the number of parameters
     passed to sendmsg (which we will use to send our ancillary data).  This
     structure uses terminology corresponding to control messages, so you'll
     see msg_control, which is the pointer to the ancillary data and controllen
     which is the size of the ancillary data array.

     So, initialize the message header that describes our ancillary/control data
     and point it to the control message/ancillary data we just allocated space
     for.
     */
  struct msghdr message;
  memset(&message, 0, sizeof(struct msghdr));
  message.msg_name = NULL;
  message.msg_namelen = 0;
  message.msg_iov = iov;
  message.msg_iovlen = 1;

  message.msg_controllen = CMSG_SPACE(sizeof(int));
  message.msg_control = cmp;

  /* A cmsghdr contains a length field that is the length of the header and
     the data.  It has a cmsg_level field corresponding to the originating
     protocol.  This takes values which are legal levels for getsockopt and
     setsockopt (here SOL_SOCKET).  We're going to use the SCM_RIGHTS type of
     cmsg, that indicates that the ancillary data array contains access rights
     that we are sending back .

     We have to put together the first (and only) cmsghdr that will describe
     the whole package we're sending.
  */
  struct cmsghdr* control_message = NULL;
  control_message = CMSG_FIRSTHDR(&message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));

  /* We also have to update the controllen in case other stuff is actually
     in there we may not be aware of (due to macros)
  */
  message.msg_controllen = control_message->cmsg_len;

  /* get a pointer to the start of the ancillary data array and
     put our file descriptor in.
  */
  Int32* control_msg =
  reinterpret_cast<Int32*>(CMSG_DATA(control_message));
  *control_msg = file_fd;

  // send the file descriptor
  if (-1 == (ret = ::sendmsg(socket_handle(), &message, flags))) {
    LOG4CXX_ERROR(logger_, "Unable to send " << strerror(errno));
    free(cmp);
  }

  free(cmp);
  return ret;
}

ssize_t ConnectedSocketImpl::recv(UInt8* buffer, size_t size, Int32 flags) {
  ssize_t ret;
  if (-1 == (ret = ::recv(socket_handle(), reinterpret_cast<void*>(buffer),
                          size, flags))) {
    LOG4CXX_ERROR(logger_, "Unable to recv " << strerror(errno));
  }

  return ret;
}

ssize_t ConnectedSocketImpl::recv(struct msghdr* msg, Int32 flags) {
  ssize_t ret;
  if (-1 == (ret = ::recvmsg(socket_handle(), msg, flags))) {
    LOG4CXX_ERROR(logger_, "Unable to recvmsg " << strerror(errno));
  }

  return ret;
}

}  // namespace net
