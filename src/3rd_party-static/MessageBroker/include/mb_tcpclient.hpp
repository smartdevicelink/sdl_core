/**
 * \file mb_tcpclient.hpp
 * \brief MessageBroker TCP client.
 * \author AKara
 */

#ifndef MB_TCPCLIENT_H 
#define MB_TCPCLIENT_H 

#include <iostream>

#include "mb_client.hpp"

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{

   /**
    * \class TcpClient
    * \brief MessageBroker TCP client.
    */
   class TcpClient : public Client
   {
      public:
         /**
         * \brief Constructor.
         * \param address remote network address or FQDN
         * \param port remote local port
         */
         TcpClient(const std::string& address, uint16_t port);

         /**
         * \brief Destructor.
         */
         virtual ~TcpClient();

         /**
         * \brief Receive data from the network.
         * \param data if data is received it will put in this reference
         * \return number of bytes received or -1 if error
         * \note This method will blocked until data comes.
         */
         virtual ssize_t Recv(std::string& data);

         /**
         * \brief Send data.
         * \param data data to send
         * \return number of bytes sent or -1 if error
         */
         ssize_t Send(const std::string& data);
   };

} /* namespace NsMessageBroker */

#endif /* MB_TCPCLIENT_H */
