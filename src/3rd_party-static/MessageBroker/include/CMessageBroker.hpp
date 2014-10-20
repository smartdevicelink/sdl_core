/**
 * \file CMessageBroker.hpp
 * \brief CMessageBroker singletone class header.
 * \author AKara
 */

#ifndef CMESSAGEBROKER_H
#define CMESSAGEBROKER_H

#include <string>

#include "CSender.hpp"

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
      INTERNAL_ERROR      = -32603, /**< Internal MessageBroker error. */
      UNSUPPORTED_RESOURCE  = 2       /**< Controller doesn't registered. */
   };

   /**
    * \brief Forward declaration of the private class.
    */
   class CMessageBroker_Private;

   /**
   * \class CMessageBroker
   * \brief Singletone CMessageBroker class implementation.
   */
   class CMessageBroker
   {
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
      void onMessageReceived(int fd, std::string& aJSONData);
      
      /**
       * \brief Test of buffer parsing.
       */
      void Test();


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
       * \brief Private constructor.
       */
      CMessageBroker();

      /**
       * \brief The PIMPL of the class.
       */
      CMessageBroker_Private *p;
   };
} /* namespace NsMessageBroker */

#endif // CMESSAGEBROKER_H

