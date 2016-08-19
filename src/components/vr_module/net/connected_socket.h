#ifndef SRC_COMPONENTS_VR_MODULE_NET_CONNECTED_SOCKET_H_
#define SRC_COMPONENTS_VR_MODULE_NET_CONNECTED_SOCKET_H_

#include <net/socket.h>

namespace net {

/**
 * Generic client socket operations abstraction
 */
class ConnectedSocket : public Socket {
 public:
  /**
   * Default constructor
   *
   * @param family  Specifies socket communication domain
   * @param address Specifies socket address. In case of NET_AF_UNIX specifies
   *pathname
   * @param port    Specifies socket port. In case of NET_AF_UNIX will be
   *omitted
   */
  ConnectedSocket(Domain domain, const char* address, UInt32 port);

  /**
   * Default Destructor
   */
  virtual ~ConnectedSocket();

  /**
     * Sets options on socket
     * Wrapper around the setsockopt system call.
     */
  virtual bool set_opt(Int32 level, Int32 optname, const void* optval,
                       socklen_t optlen);

  /**
   * Closes socket
   */
  virtual void close();

  /**
   * Shudowns socket using SHUT_RDWR option
   */
  virtual void shutdown();

  /**
   * Sets the blocking mode of this socket. If set to nonblocking,
   * read/write calls will always return immediately, but possibly
   * not all of the data has been read/written.
   *
   * @param is_blocking Whether to operate in blocking mode or not.
   */
  virtual void set_blocking_mode(bool is_blocking);

  /**
   * Sends size bytes over socket.
   *
   * @param buffer Data to send
   * @param size   Number of bytes to send
   * @param flags  The flags argument
   *
   * @return number of bytes sent.
   */
  virtual ssize_t send(const UInt8* buffer, size_t size, Int32 flags) = 0;

  /**
   * Sends file descriptor in control messages(ancillary data)
   *
   * @param file_fd The file descriptor to send
   * @param flags   The flags argument
   *
   * @return number of bytes sent.
   */
  virtual ssize_t send(Int32 file_fd, Int32 flags) = 0;

  /**
   * Reads bytes from socket. This function call may return a value lower than
   *size in case of
   * signals interrupting the call.
   *
   * @param buffer buffer for read data
   * @param size Maximum number of bytes that could be read
   * @param flags  The flags argument
   *
   * @return number of bytes read.
   */
  virtual ssize_t recv(UInt8* buffer, size_t size, Int32 flags) = 0;

  /**
   * Reads from socket control-related messages
   *
   * @param msg msghdr structure
   * @param flags  The flags argument
   *
   * @return number of bytes read.
   */
  virtual ssize_t recv(struct msghdr* msg, Int32 flags) = 0;

  virtual bool connect() = 0;

 protected:
  ConnectedSocket(Domain domain, Int32 handle);
};

}  // namespace net

#endif  // SRC_COMPONENTS_VR_MODULE_NET_CONNECTED_SOCKET_H_
