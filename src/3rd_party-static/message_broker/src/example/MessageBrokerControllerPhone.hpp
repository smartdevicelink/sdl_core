/**
 * \file MessageBrokerControllerPhone.hpp
 * \brief MessageBroker Controller Phone.
 * \author AKara
 */

#ifndef MB_CONTROLLERPHONE_H 
#define MB_CONTROLLERPHONE_H 

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
    * \class CMessageBrokerControllerPhone
    * \brief MessageBroker Controller Phone.
    */
   class CMessageBrokerControllerPhone : public CMessageBrokerController
   {
      public:
         /**
         * \brief Constructor.
         * \param address remote network address or FQDN
         * \param port remote local port
         */
         CMessageBrokerControllerPhone(const std::string& address, uint16_t port);

         /**
         * \brief Destructor.
         */
         ~CMessageBrokerControllerPhone();

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
      public://Notifications
         /**
         * \brief Notifies Call Status changing.
         * \param callStatus status of current active call
         */
         void onCallStatusChanged(int callStatus);

         /**
         * \brief Notifies Contacts list updated.
         */
         void onContactsUpdated();

         /**
         * \brief Notifies History list updated.
         */
         void onHistoryUpdated();
      private:
         /**
         * \brief Makes call.
         * \param phoneNumber number for call.
         * \param response container for response
         */
         void makeCall(std::string phoneNumber, Json::Value& response);

         /**
         * \brief Ends call.
         * \param phoneNumber number of call.
         * \param response container for response
         */
         void endCall(std::string phoneNumber, Json::Value& response);

         /**
         * \brief Holds call.
         * \param phoneNumber number of call.
         * \param response container for response
         */
         void holdCall(std::string phoneNumber, Json::Value& response);

         /**
         * \brief Gets contacts.
         * \param firstLetter first letter of list.
         * \param offset offset from first item
         * \param numberOfItems number of expected items
         * \param response container for response
         */
         void getContacts(std::string firstLetter, int offset, int numberOfItems, Json::Value& response);

         /**
         * \brief Gets history.
         * \param typeOfContacts type of contacts (incoming/outgoing/missed/all calls).
         * \param offset offset from first item
         * \param numberOfItems number of expected items
         * \param response container for response
         */
         void getHistory(std::string typeOfContacts, int offset, int numberOfItems, Json::Value& response);

   };
}/* namespace NsMessageBroker */
#endif /* MB_CONTROLLERPHONE_H */