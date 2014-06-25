/**
 * \file mb_server.hpp
 * \brief MessageBroker server.
 * \author AKara
 */

#ifndef MB_SERVER_H
#define MB_SERVER_H

#include "networking.h"

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{

   /**
   * \class Server
   * \brief Abstract MessageBroker server.
   */
   class Server
   {
      public:
         /**
         * \brief Constructor.
         * \param address network address or FQDN to bind
         * \param port local port to bind
         */
         Server(const std::string& address, uint16_t port);

         /**
         * \brief Destructor.
         */
         virtual ~Server();

         /**
         * \brief Wait message.
         *
         * This function do a select() on the socket and Process() immediately 
         * the message.
         * \param ms millisecond to wait (0 means infinite)
         */
         virtual void WaitMessage(uint32_t ms) = 0;

         /**
         * \brief Get socket descriptor.
         * \return socket descriptor.
         */
         int GetSocket() const;

         /**
         * \brief Get the address.
         * \return address or FQDN
         */
         std::string GetAddress() const;

         /**
         * \brief Get the port.
         * \return local port
         */
         uint16_t GetPort() const;

         /**
         * \brief Bind the socket.
         * \return true if success, false otherwise
         */
         bool Bind();

         /**
         * \brief Receive data from the network and process it.
         * \param fd file descriptor on which receive
         * \return true if message has been correctly received, processed and
         * response sent, false otherwise (mainly send/receive error)
         * \note This method will blocked until data comes.
         */
         virtual bool Recv(int fd) = 0;
        
         /**
         * \brief Close socket.
         * \note It should be overriden for connection-oriented protocol 
         * like TCP to properly close all client sockets.
         */
         virtual void Close();

      protected:
         /**
         * \brief Socket descriptor.
         */
         int m_sock;

         /**
         * \brief Transport protocol of the socket.
         */
         enum networking::TransportProtocol m_protocol;

         /**
         * \brief Transport protocol of the socket.
         */

      private:
         /**
         * \brief Network address or FQDN.
         */
         std::string m_address;

         /**
         * \brief Local port.
         */
         uint16_t m_port;

         /**
         * \brief Encapsulated format.
         */
   };

} /* namespace NsMessageBroker */

#endif /* MB_SERVER_H */

