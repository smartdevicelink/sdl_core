#ifndef SRC_COMPONENTS_VR_MODULE_NET_CONNECTED_SOCKET_IMPL_H_
#define SRC_COMPONENTS_VR_MODULE_NET_CONNECTED_SOCKET_IMPL_H_

#include <net/connected_socket.h>

namespace net {

/**
 * Generic client socket operations implementation
 */
class ConnectedSocketImpl : public virtual ConnectedSocket {
 public:
  /**
   * Default constructor
   *
   * @param address Specifies socket address
   * @param port    Specifies socket port
   */
  ConnectedSocketImpl(const char* address, UInt32 port);

  friend class ServerSocketImpl;

  /**
   * Provides connected socket to the remote host
   *
   * @param address Specifies socket address
   * @param port    Specifies socket port
   * @param attempts    attempts to connect to host.
   * It attempts = 0, try to connecto to  host infinitely
   *
   * @return ConnectedSocket object, that must be deleted by caller
   */
  static ConnectedSocket* ConnectToHost(const char* address, UInt32 port,
                                        UInt32 attempts = 30);

  /**
   * Provides connected socket to the local host
   *
   * @param path_name Specifies socket path
   *
   * @return ConnectedSocket object, that must be deleted by caller
   */
  static ConnectedSocket* ConnectToHost(const char* path_name);

  /**
   * Default Destructor
   */
  virtual ~ConnectedSocketImpl();

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
  virtual ssize_t send(const UInt8* buffer, size_t size, Int32 flags);

  /**
   * Sends file descriptor in control messages(ancillary data)
   *
   * @param file_fd The file descriptor to send
   * @param flags   The flags argument
   *
   * @return number of bytes sent.
   */
  virtual ssize_t send(Int32 file_fd, Int32 flags);

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
  virtual ssize_t recv(UInt8* buffer, size_t, Int32 flags);

  /**
   * Reads from socket control-related messages
   *
   * @param msg msghdr structure
   * @param flags  The flags argument
   *
   * @return number of bytes read.
   */
  virtual ssize_t recv(struct msghdr* msg, Int32 flags);

 protected:
  /**
 * Connects a client to a remote server socket.
 *
 * @return TRUE in case of success, otherwise FALSE.
 *
 */
  virtual bool connect();

 private:
  /**
   * Default constructor for AF_UNIX domain sockets
   *
   * @param path_name   Specifies socket communication domain
   */
  explicit ConnectedSocketImpl(const char* path_name);

  /**
   * Constructor used by Server to create accepted connection
   *
   * @param domain    Specifies socket domain
   * @param handle    Specifies socket handle returned by accept call
   */
  ConnectedSocketImpl(Domain domain, Int32 handle);
};

}  // namespace net

#endif  // SRC_COMPONENTS_VR_MODULE_NET_CONNECTED_SOCKET_IMPL_H_
