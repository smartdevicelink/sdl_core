/*
 * mb_mqueue.cpp
 *
 *  Created on: Jun 25, 2013
 *      Author: adderleym
 */

#include "mb_mqclient.hpp"
#include "MBDebugHelper.h"

namespace NsMessageBroker
{

   MqClient::MqClient()
   :m_sndHandle(-1),
    m_rcvHandle(-1),
    m_send(std::string("")),
    m_recv(std::string(""))
   {
   }

   MqClient::MqClient(const std::string& queue_name_snd, const std::string& queue_name_rcv)
   :m_sndHandle(-1),
    m_rcvHandle(-1),
    m_send(queue_name_snd),
    m_recv(queue_name_rcv)
   {
   }


   MqClient::~MqClient()
   {
      if(m_sndHandle != -1)
      {
          ::mq_close(m_sndHandle);
          m_sndHandle = -1;
      }

      if(m_rcvHandle != -1)
      {
          ::mq_close(m_rcvHandle);
          m_rcvHandle = -1;
      }
   }

   bool MqClient::MqOpen()
   {
	  struct mq_attr attributes;
	  attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
	  attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
	  attributes.mq_flags = 0;

	  m_sndHandle = ::mq_open(m_send.c_str(), O_WRONLY|O_CREAT|O_NONBLOCK, 0666, &attributes);
	  m_rcvHandle = ::mq_open(m_recv.c_str(), O_RDONLY|O_CREAT, 0666, &attributes);

 	  return ((m_sndHandle != -1) && (m_rcvHandle != -1)) ? true : false;
   }


   ssize_t MqClient::Send(const std::string& data)
   {

		std::string rep = data;
		const char* ptrBuffer = rep.c_str();

		if(rep.empty())
		{
			return -1;
		}

		int retVal = ::mq_send(m_sndHandle, ptrBuffer, rep.length(), 0);

		return (retVal == -1) ? retVal : rep.length();

   }


   ssize_t MqClient::Recv(std::string& data)
   {
		char buf[MAX_QUEUE_MSG_SIZE];
		ssize_t length=0;

		if ((length = ::mq_receive(m_rcvHandle, buf, sizeof(buf), 0)) == -1)
		{
			return -1;
		}

		data = std::string(&buf[1], length);

		return length;
   }

} /* namespace NsMessageBroker */
