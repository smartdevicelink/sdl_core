/**
 * \file CMessageBroker.hpp
 * \brief CMessageBroker singletone class header.
 * \author AKara
 */

#ifndef CMESSAGEBROKER_H
#define CMESSAGEBROKER_H

#include <deque>
#include <map>
#include <string>

#include "CSender.hpp"
#include "CMessageBrokerRegistry.hpp"

#include "json/json.h"

#include "system.h"

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker 
{
   /**
    * \enum ErrorCode
    * \brief MessageBroker error codes.
    * \note Value from -32099 to -32000 are reserved for implementation-defined server-errors.
    */
   enum ErrorCode
   {
      CONTROLLER_EXISTS   = -32000, /**< Controller has been already registered. */
      SUBSCRIBTION_EXISTS = -32001, /**< Property has been already subscribed by this controller. */
      PARSING_ERROR       = -32700, /**< Invalid JSON. An error occurred on the server while parsing the JSON text. */
      INVALID_REQUEST     = -32600, /**< The received JSON not a valid MessageBroker Request. */
      METHOD_NOT_FOUND    = -32601, /**< The requested remote-procedure does not exist / is not available. */
      INVALID_PARAMS      = -32602, /**< Invalid method parameters. */
      INTERNAL_ERROR      = -32603  /**< Internal MessageBroker error. */
   }; 

   /**
   * \class CMessageBroker
   * \brief Singletone CMessageBroker class implementation.
   */
   class CMessageBroker
   {
   private:
      /**
      * \class CMessage
      * \brief CMessage class implementation.
      */
      class CMessage
      {
      public:
         /**
         * \brief Constructor.
         */
         CMessage(int aSenderFp, Json::Value aMessage)
         {
            mSenderFd = aSenderFp;
            mMessage = aMessage;
         }

         /**
         * \brief Destructor.
         */
         ~CMessage()
         {
         }

         /**
         * \brief getter for Json::Value message.
         * \return Json::Value message.
         */
         Json::Value getMessage() const
         {
            return mMessage;
         }

         /**
         * \brief getter for sender FileDescriptor.
         * \return sender FileDescriptor.
         */
         int getSenderFd() const
         {
            return mSenderFd;
         }
      private:
         /**
         * \brief sender FileDescriptor.
         */
         int mSenderFd;

         /**
         * \brief Json::Value message.
         */
         Json::Value mMessage;
      };
   public:
      /**
      * \brief Singletone instantiator.
      * \return pointer to CMessageBroker instance.
      */
      static CMessageBroker* getInstance();

      /**
      * \brief Destructor.
      */
      ~CMessageBroker();

      /**
      * \brief Receive data from TCP server (from client).
      * \param fd FileDescriptor of socket.
      * \param aJSONData JSON string.
      */
      void onMessageReceived(int fd, std::string aJSONData);

      /**
      * \brief Starts MessageBroker.
      */
      void startMessageBroker(CSender* pSender);

      /**
      * \brief Stops MessageBroker.
      */
      void stopMessageBroker();

      /**
      * \brief Method for thread with que processing.
      */
      void* MethodForThread(void * arg);
    private:

      /**
       * \brief Check if que empty (Thread safe).
       * \return True when empty.
       */
      bool isEventQueueEmpty();

      /**
       * \brief Pop message from que (Thread safe).
       * \return Pointer to CMessage.
       */
      CMessage* popMessage();

      /**
       * \brief Push message to que (Thread safe).
       * \param pMessage pointer to new CMessage object.
       */
      void pushMessage(CMessage* pMessage);

      /**
       * \brief gets  destination component name.
       * \param pMessage JSON message.
       * \return string destination component name.
       */
      std::string getDestinationComponentName(CMessage* pMessage);

      /**
       * \brief gets  method name.
       * \param pMessage JSON message.
       * \return string method name.
       */
      std::string getMethodName(CMessage* pMessage);

      /**
       * \brief checks is message notification or not.
       * \param pMessage JSON message.
       * \return true if notification.
       */
      bool isNotification(CMessage* pMessage);

      /**
       * \brief checks is message response or not.
       * \param pMessage JSON message.
       * \return true if response.
       */
      bool isResponse(CMessage* pMessage);

      /**
       * \brief checks message.
       * \param pMessage JSON message.
       * \param error JSON message to fill in case of any errors.
       * \return true if message is good.
       */
      bool checkMessage(CMessage* pMessage, Json::Value& error);

      /**
       * \brief Process internal MessageBrocker message
       *
       * \brief Register controller in MessageBroker.
       *  Use following JSON command to register new component:
       * \code
       *  {"jsonrpc": "2.0", "method": "MB.registerComponent", "params": "<ComponentName>"}
       * \endcode
       *
       * \brief Unregister controller in MessageBroker.
       *  Use following JSON command to unregister component:
       * \code
       *  {"jsonrpc": "2.0", "method": "MB.unregisterComponent", "params": "<ComponentName>"}
       * \endcode
       *
       * \brief Subscribe controller on property change.
       *  Use following JSON command to subscribe to notifications:
       * \code
       *  {"jsonrpc": "2.0", "method": "MB.subscribeTo", "params": "<ComponentName>.<NotificationName>"}
       * \endcode
       *
       * \brief Unsubscribe controller from property change.
       *  Use following JSON command to unsubscribe from notifications:
       * \code
       *  {"jsonrpc": "2.0", "method": "MB.unsubscribeFrom", "params": "<ComponentName>.<NotificationName>"}
       * \endcode
       *
       * \param pMessage JSON message.
       */
      void processInternalMessage(CMessage* pMessage);

      /**
       * \brief process external message.
       * \param pMessage JSON message.
       */
      void processExternalMessage(CMessage* pMessage);

      /**
       * \brief process response.
       * \param pMessage JSON message.
       */
      void processResponse(CMessage* pMessage);

      /**
       * \brief Process notification message.
       * \brief Notify subscribers about property change.
       * expected notification format example:
       * \code
       * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
       * \endcode
       * \param pMessage JSON message.
       */
      void processNotification(CMessage* pMessage);

      /**
       * \brief send error message.
       * \param pMessage JSON message.
       */
      void processError(CMessage* pMessage);

      /**
       * \brief send Json message.
       * \param fd FileDescriptor of socket.
       * \param message JSON message.
       */
      void sendJsonMessage(int fd, Json::Value message);

      /**
       * \brief push message to wait response que.
       * \param pMessage JSON message.
       */
      void pushMessageToWaitQue(CMessage* pMessage);

      /**
       * \brief Returns start position for Id's generator of controller.
       * \return start position for Id's generator of controller (1000 id's).
       */
      int getNextControllerIdDiapason()
      {
         return 1000 * mControllersIdCounter++;
      }

       /**
       * \brief pop message from wait response que.
       * \param pMessage JSON message.
       */
      int popMessageFromWaitQue(CMessage* pMessage);

       private:
      /**
      * \brief Constructor.
      */
      CMessageBroker();

      /**
      * \brief Que of messages.
      */
      std::deque<CMessage*> mMessagesQueue;

      /**
      * \brief Counter of messages Id's diapason for the next controllers
      * From mControllersIdCounter*1000 to mControllersIdCounter*1000+999.
      */
      int mControllersIdCounter;

      /**
      * \brief Que of messages which are waiting the response in format: MessageId:SenderFd.
      */
      std::map<std::string, int> mWaitResponseQueue;

      /**
      * \brief Pointer to sender.
      */
      CSender* mpSender;

      /**
      * \brief Pointer to registry.
      */
      CMessageBrokerRegistry* mpRegistry;

      /**
      * \brief JSON reader.
      */
     Json::Reader m_reader;

     /**
      * \brief JSON writer.
      */
     Json::FastWriter m_writer;

      /**
      * \brief Messages que mutex.
      */
      System::Mutex mMessagesQueueMutex;
      };
} /* namespace NsMessageBroker */

#endif // CMESSAGEBROKER_H

