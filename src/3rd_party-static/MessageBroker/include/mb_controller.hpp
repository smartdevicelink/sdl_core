/**
 * \file mb_controller.hpp
 * \brief MessageBroker Controller.
 * \author AKara
 */

#ifndef MB_CONTROLLER_H 
#define MB_CONTROLLER_H 

#include <iostream>

#include "json/json.h"

#include "mb_tcpclient.hpp"
#include "utils/lock.h"
#include "utils/atomic_object.h"

#include <cstring> 

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{

   /**
    *\class CMessageBrokerController
    * \brief MessageBroker Controller.
    */
   class CMessageBrokerController : public TcpClient
   {
   public:
      /**
      * \brief Constructor.
      * \param address remote network address or FQDN
      * \param port remote local port
      * \param name name of component
      */
      CMessageBrokerController(const std::string& address, uint16_t port, std::string name);


      /**
      * \brief Destructor.
      */
      ~CMessageBrokerController();

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

      /**
       * \brief send Json message.
       * \param message JSON message.
       */
      void sendJsonMessage(Json::Value& message);

      /**
      * \brief generates new message id from diapason mControllersIdStart - (mControllersIdStart+999).
      * \return next id for message
      */
      int getNextMessageId();

      /**
      * \brief generates new message with id, jsonrpc version.
      * \param root container for prepared message
      */
      void prepareMessage(Json::Value& root);

      /**
      * \brief generates new message with id, jsonrpc version.
      * \note Doesn't change/add fields id and jsonrpc
      * \param errCode error code
      * \param errMessage string of message
      * \param error container for prepared error message
      */
      void prepareErrorMessage(int errCode, std::string errMessage, Json::Value& error);

      /**
       * \brief gets  destination component name.
       * \param root JSON message.
       * \return string destination component name.
       */
      std::string getDestinationComponentName(Json::Value& root);

      /**
       * \brief gets  method name.
       * \param root JSON message.
       * \return string method name.
       */
      std::string getMethodName(Json::Value& root);

      /**
       * \brief checks is message notification or not.
       * \param root JSON message.
       * \return true if notification.
       */
      bool isNotification(Json::Value& root);

      /**
       * \brief checks is message response or not.
       * \param root JSON message.
       * \return true if response.
       */
      bool isResponse(Json::Value& root);

      /**
       * \brief searches Method by id in mWaitResponseQueue.
       * \param id id of incoming JSON message.
       * \return string method name or "" in case not found.
       */
      std::string findMethodById(std::string id);

      /**
       * \brief register controller on the server.
       * \param id message id for JSON message due the id diapason hasn't been received.
       */
      void registerController(int id = 0);

      /**
       * \brief unregister controller on the server.
       */
      void unregisterController();

      /**
       * \brief subscribes controller to the property changing.
       * \param property property name in format ComponentName.PropertyName.
       */
      void subscribeTo(std::string property);

      /**
       * \brief unsubscribes controller from the property changing.
       * \param property property name in format ComponentName.PropertyName.
       */
      void unsubscribeFrom(std::string property);

      /**
       * \brief pure virtual method to process response.
       * \param method method name which has been called.
       * \param root JSON message.
       */
      virtual void processResponse(std::string method, Json::Value& root) = 0;

      /**
       * \brief pure virtual method to process request.
       * \param root JSON message.
       */
      virtual void processRequest(Json::Value& root) = 0;

      /**
       * \brief Process notification message.
       * \brief Notify subscribers about property change.
       * expected notification format example:
       * \code
       * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
       * \endcode
       * \param root JSON message.
       */
      virtual void processNotification(Json::Value& root) = 0;

      /**
       * \brief Checks message.
       * \param root JSON message.
       * \param error JSON message to fill in case of any errors.
       * \return true if message is good.
       */
      bool checkMessage(Json::Value& root, Json::Value& error);
      
      /**
       * \brief Returns name of Controller.
       * \return name of controller.
       */
      std::string getControllersName();

      /**
      * \brief Method for receiving thread.
      */
      void* MethodForReceiverThread(void * arg);

      virtual void exitReceivingThread() {
        Close();
        stop = true;
      }

   protected:
      /**
       * @brief flag top stop thread
       */
      sync_primitives::atomic_bool stop;

   private:
      /**
      * \brief Method for receiving messages without tcp packeting.
      * \param message received data
      */
      void onMessageReceived(Json::Value message);
      /**
      * \brief Start value of id's diapason.
      */
      std::string m_receivingBuffer;

      /**
      * \brief Start value of id's diapason.
      */
      int mControllersIdStart;

      /**
      * \brief Current id's value.
      */
      int mControllersIdCurrent;

      /**
      * \brief Already sent messages Methods to recognize esponses: MessageId:MethodName.
      */
      std::map<std::string, std::string> mWaitResponseQueue;

      /**
      * \brief Name of component.
      */
      std::string mControllersName;

      /**
      * \brief JSON reader.
      */
     Json::Reader m_reader;

     /**
      * \brief JSON writer.
      */
     Json::FastWriter m_writer;

     /**
      * \brief JSON writer.
      */
     Json::FastWriter m_receiverWriter;
     /*
      * @brief mutex for mWaitResponseQueue
      */
     sync_primitives::Lock       queue_lock_;
   };
} /* namespace NsMessageBroker */
#endif /* MB_CONTROLLER_H */
