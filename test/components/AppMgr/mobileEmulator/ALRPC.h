#ifndef ALRPC_H
#define ALRPC_H

#include "JSONHandler/ALRPCObjects/Marshaller.h"
#include "JSONHandler/IRPCMessagesObserver.h"
#include "JSONHandler/JSONHandler.h"
#include "LoggerHelper.hpp"

/**
 * \namespace NsMobileEmulator
 * \brief MobileEmulator related functions.
 */
namespace NsMobileEmulator
{

    /**
     * \brief The ALRPC class acts as a replacement fot JSONHandler in a test enviroment
     */
    class ALRPC : public JSONHandler
    {
    public:

        /**
        * \brief Constructor.
        */
        ALRPC( );

        /**
        * \brief Destructor.
        */
        virtual ~ALRPC();

    protected:

        /**
         * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
         * \param messagesObserver Pointer to object of the class implementing IRPCMessagesObserver
         * \sa IRPCMessagesObserver
         */
        void setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver );

        /**
         * \brief Method for sending message to Mobile Application.
         * \param message Pointer to base class of AppLink Json object
         * to be serialized to Json message and sent to mobile App.
         * \param connectionId ID of the connection the message was received within.
         * \param sessionId ID of the session the message was received within.
         */
        void sendRPCMessage( const NsAppLinkRPC::ALRPCMessage * message, int connectionId, unsigned char sessionId );

        /**
        * \brief Get message observer instance
        * \return message observer instance
        */
        IRPCMessagesObserver* getRPCMessageObserver() const;

        /**
         * \brief Gets invoked upon a mobile RPC message gets received from HMI
         * \param message Pointer to base class of AppLink Json object
         * \param connectionId ID of the connection the message was received within.
         * \param sessionId ID of the session the message was received within.
         */
        virtual void messageReceivedFromHmiCallback( const NsAppLinkRPC::ALRPCMessage * message, int connectionId, unsigned char sessionId )=0;

        /**
          *\brief For logging.
        */
        static log4cplus::Logger           mLogger;

    private:
        /**
          *\brief Points on instance of class implementing RPC handling (Application Manager).
        */
        IRPCMessagesObserver *             mMessagesObserver;
    };

}

#endif // ALRPC_H
