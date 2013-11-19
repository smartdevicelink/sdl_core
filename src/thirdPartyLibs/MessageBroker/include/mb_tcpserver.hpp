/**
 * \file mb_tcpserver.hpp
 * \brief MessageBroker TCP server.
 * \author AKara
 */

#ifndef MB_TCPSERVER_H
#define MB_TCPSERVER_H

#include <list>
#include <map>

#include "mb_server.hpp"
#include "CMessageBroker.hpp"
#include "CSender.hpp"
#include "websocket_handler.hpp"

#define RECV_BUFFER_LENGTH 4097
#define MAX_RECV_BUFFER_LENGTH 100000
#define MAX_RECV_DATA 4096

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{
   /**
   * \class TcpServer
   * \brief MessageBroker TCP server implementation.
   */
   class TcpServer : public Server, public CSender
   {
      public:
         /**
         * \brief Constructor.
         * \param address network address or FQDN to bind
         * \param port local port to bind
         * \param pMessageBroker pointer to MessageBroker
         */
         TcpServer(const std::string& address, uint16_t port, NsMessageBroker::CMessageBroker* pMessageBroker);

         /**
         * \brief Destructor.
         */
         virtual ~TcpServer();

         /**
         * \brief Receive data from the network and process it.
         * \param fd socket descriptor to receive data
         * \return true if message has been correctly received, processed and
         * response sent, false otherwise (mainly send/recv error)
         * \note This method will blocked until data comes.
         */
         virtual bool Recv(int fd);

         /**
         * \brief Send data.
         * \param fd file descriptor of the client TCP socket
         * \param data data to send
         * \return number of bytes sent or -1 if error
         */
         virtual ssize_t Send(int fd, const std::string& data);

         /**
         * \brief Wait message.
         *
         * This function do a select() on the socket and Process() immediately 
         * the message.
         * \param ms millisecond to wait (0 means infinite)
         */
         virtual void WaitMessage(uint32_t ms);

         /**
         * \brief Put the TCP socket in LISTEN state.
         */
         bool Listen() const;

         /**
         * \brief Accept a new client socket.
         * \return -1 if error, 0 otherwise
         */
         bool Accept();

         /**
         * \brief Close listen socket and all client sockets.
         */
         void Close();

         /**
         * \brief Method for thread with WaitMessage.
         */
         void* MethodForThread(void * arg);

      private:
         /**
         * \brief Checks if incoming messages are websocket request.
         * \param fd 
         * \param pReceivingBuffer string receiving buffer pointer
         * \return true if it is a websocket handshake
         */
         bool checkWebSocketHandShake(int fd, std::string* pReceivingBuffer);

         /**
         * \brief Checks if incoming messages are websocket request.
         * \param fd file descriptor of the client TCP socket
         * \return true if it is a websocket
         */
         bool isWebSocket(int fd);

         /**
         * \brief Gets buffer for socket which received data.
         * \param fd socket file descriptor
         */
         std::string* getBufferFor(int fd);
      private:
         /**
         * \brief WebSocket clients fd's list.
         */
         std::list<int> m_WebSocketClients; 

         /**
         * \brief Just accepted clients fd's list.
         */
         std::list<int> m_AcceptedClients; 

         /**
         * \brief Receiving buffers map SocketFd:bufferPointer.
         */
         std::map <int, std::string*> m_receivingBuffers; 

         /**
         * \brief List of disconnected sockets to be purged.
         */
         std::list<int> m_purge;
         
         /**
         * \brief MessageBroker pointer.
         */
         NsMessageBroker::CMessageBroker* mpMessageBroker;
         
         /**
         * \brief WebSocket handler.
         */
         CWebSocketHandler mWebSocketHandler;
   };

} /* namespace NsMessageBroker */

#endif /* MB_TCPSERVER_H */
