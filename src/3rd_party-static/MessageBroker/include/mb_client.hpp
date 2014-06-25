/**
 * \file mb_client.hpp
 * \brief MessageBroker client.
 * \author AKara
 */

#ifndef MB_CLIENT_H
#define MB_CLIENT_H

#if _MSC_VER >= 1400 // VC++ 8.0
typedef int ssize_t;
#endif 

#include "networking.h"

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{

   /**
    * \class Client
    * \brief Abstract MessageBroker client.
    */
   class Client
   {
      public:
         /**
         * \brief Constructor.
         * \param address remote network address or FQDN to contact
         * \param port remote local port to contact
         */
         Client(const std::string& address, uint16_t port);

         /**
         * \brief Destructor.
         */
         virtual ~Client();

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
         * \brief Connect to the remote machine
         * \return true if success, false otherwise
         * \note on connectionless protocol like UDP, this function
         * always returns true even if remote peer is not reachable.
         */
         virtual bool Connect();

         /**
         * \brief Receive data from the network.
         * \param data if data is received it will put in this reference
         * \return number of bytes received or -1 if error
         * \note This method will blocked until data comes.
         */
         virtual ssize_t Recv(std::string& data) = 0;

         /**
         * \brief Close socket.
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
         * \brief Remote socket address.
         */
         struct sockaddr_storage m_sockaddr;

         /**
         * \brief Remote socket address length.
         */
         socklen_t m_sockaddrlen;

         /**
         * \brief Protected constructor so our inherited classes
         * don't need to call the default constructor
         */
         Client();

         /**
         * \brief Protected SetAddress so out inherited classes
         * don't need to call the default constructor
         */
         void SetAddress(const std::string& address);

         /**
         * \brief Protected SetPort so out inherited classes
         * don't need to call the default constructor
         */
         void SetPort(uint16_t port);
      
      private:  
    
         /**
         * \brief Network address or FQDN.
         */
         std::string m_address;

         /**
         * \brief Local port.
         */
         uint16_t m_port;
   };

} /* namespace NsMessageBroker */

#endif /* MB_CLIENT_H */
