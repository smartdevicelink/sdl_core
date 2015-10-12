/**
 * \file mb_client.cpp
 * \brief MessageBroker client.
 * \author AKara
 */
#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include <cstring>

#include "mb_client.hpp"

namespace NsMessageBroker
{

   Client::Client()
   {
   }

   Client::Client(const std::string& address, uint16_t port)
   {
      m_sock = -1;
      m_address = address;
      m_port = port;
      memset(&m_sockaddr, 0x00, sizeof(struct sockaddr_storage));
      m_sockaddrlen = 0;
   }

   Client::~Client()
   {
      if(m_sock != -1)
      {
        Close();
      }
   }

   int Client::GetSocket() const
   {
      return m_sock;
   }

   std::string Client::GetAddress() const
   {
      return m_address;
   }

   void Client::SetAddress(const std::string& address)
   {
     m_address = address;
   }

   void Client::SetPort(uint16_t port)
   {
     m_port = port;
   }

   uint16_t Client::GetPort() const
   {
      return m_port;
   }

   bool Client::Connect()
   {
      m_sock = networking::connect(m_protocol, GetAddress(), GetPort(), &m_sockaddr, &m_sockaddrlen);

      return (m_sock != -1) ? true : false;
   }

   void Client::Close()
   {
#ifdef OS_WIN32
		 // do nothing
	   shutdown(m_sock, 2);
	   closesocket(m_sock);
#else
      shutdown(m_sock, SHUT_RDWR);
	  close(m_sock);
#endif
      
      m_sock = -1;
   }

} /* namespace NsMessageBroker */
