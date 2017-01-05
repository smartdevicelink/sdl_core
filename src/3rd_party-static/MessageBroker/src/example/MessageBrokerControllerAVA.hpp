/**
 * \file MessageBrokerControllerAVA.hpp
 * \brief MessageBroker Controller AVA.
 * \author AKara
 */

#ifndef MB_CONTROLLERAVA_H 
#define MB_CONTROLLERAVA_H 

#include <iostream>

#include "json/json.h"

#include "mb_controller.hpp"

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{
   /**
    * \class CMessageBrokerControllerAVA
    * \brief MessageBroker Controller.
    */
   class CMessageBrokerControllerAVA : public CMessageBrokerController
   {
      public:
         /**
         * \brief Constructor.
         * \param address remote network address or FQDN
         * \param port remote local port
         */
         CMessageBrokerControllerAVA(const std::string& address, uint16_t port);

         /**
         * \brief Destructor.
         */
         ~CMessageBrokerControllerAVA();

          /**
         * \brief process request.
         * \param root JSON message.
         */
        void processRequest(Json::Value& root);

         /**
         * \brief process notification.
         * \param root JSON message.
         */
        void processNotification(Json::Value& root);

         /**
         * \brief process response.
         * \param method method name which has been called.
         * \param root JSON message.
         */
         void processResponse(std::string method, Json::Value& root);

         /**
         * \brief sends message to the phone to make call.
         * \param phoneNumber number for call.
         */
         void makeCall(std::string phoneNumber);

   };
}/* namespace NsMessageBroker */
#endif /* MB_CONTROLLERAVA_H */