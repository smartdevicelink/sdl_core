/**
 * \file mb_tcpclient.cpp
 * \brief MessageBroker TCP client.
 * \author AKara
 */

#include "mb_tcpclient.hpp"
#include "MBDebugHelper.h" 

namespace NsMessageBroker
{

   TcpClient::TcpClient(const std::string& address, uint16_t port) : Client(address, port)
   {
      m_protocol = networking::TCP;
   }

   TcpClient::~TcpClient()
   {
   }

   ssize_t TcpClient::Send(const std::string& data)
   {
      std::string rep = data;
      int bytesToSend = rep.length();
      const char* ptrBuffer = rep.c_str();
      do
      {
         int retVal = send(m_sock, ptrBuffer, bytesToSend, 0);
         if(retVal == -1)
         {
            return -1;
         }
         bytesToSend -= retVal;
         ptrBuffer += retVal;
      }while(bytesToSend > 0); 
      return rep.length();
   }

   ssize_t TcpClient::Recv(std::string& data)
   {
      char buf[1500];
      ssize_t nb = -1;

      if((nb = ::recv(m_sock, buf, sizeof(buf), 0)) == -1)
      {
         std::cerr << "Error while receiving" << std::endl;
         return -1;
      }

      data = std::string(buf, nb);
      DBG_MSG(("Received from server: %s\n", data.c_str()));

      return nb;
   }

} /* namespace NsMessageBroker */

