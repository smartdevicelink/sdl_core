/**
 * \file mb_mqclient.hpp
 * \brief MessageBroker MQUEUE client.
 * \author adderleym
 */

#ifndef MB_MQCLIENT_H
#define MB_MQCLIENT_H

#include <stdio.h>
#include <mqueue.h>
#include <string>
#include <string.h>

#include "mb_client.hpp"

#define MAX_QUEUE_NAME_SIZE     24
#define MAX_QUEUE_MSG_SIZE 		4095
#define MSGQ_MAX_MESSAGES		128
#define MSGQ_MESSAGE_SIZE 		MAX_QUEUE_MSG_SIZE

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
   class MqClient : public Client
   {
      public:
         /**
         * \brief Constructor.
         */
         MqClient(const std::string& queue_name_snd, const std::string& queue_name_rcv);

         /**
         * \brief Destructor.
         */
         virtual ~MqClient();

         bool MqOpen();

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


      protected:
         /**
         * \brief Protected constructor so our inherited classes
         * don't need to call the default constructor
         */
         MqClient();


      private:

         mqd_t m_sndHandle;
         mqd_t m_rcvHandle;

         std::string m_send;
         std::string m_recv;
   };

} /* namespace NsMessageBroker */


#endif /* MB_MQUEUE_H */
