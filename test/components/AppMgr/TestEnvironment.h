#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H

#include "mobileEmulator/ALRPC.h"

namespace NsTest
{
    /**
     * \brief The TestEnvironment class acts as a main spot of sending/receiving mobile RPC messages
     */
    class TestEnvironment : public NsMobileEmulator::ALRPC
    {
    public:

        /**
         * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
         * \param messagesObserver Pointer to object of the class implementing IRPCMessagesObserver
         * \sa IRPCMessagesObserver
         */
        void setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver );

        /**
         * \brief send a message to HMI via applinkcore
         * \param message Pointer to base class of AppLink Json object
         * \param connectionId ID of the connection the message was received within.
         * \param sessionId ID of the session the message was received within.
         */
        void sendToHmi(NsAppLinkRPC::ALRPCMessage *message, int connectionId, unsigned char sessionId );

        /**
         * \brief Gets invoked upon a mobile RPC message gets received from HMI
         * \param message Pointer to base class of AppLink Json object
         * \param connectionId ID of the connection the message was received within.
         * \param sessionId ID of the session the message was received within.
         */
        virtual void messageReceivedFromHmiCallback( const NsAppLinkRPC::ALRPCMessage * message, int connectionId, unsigned char sessionId );
    };

}

#endif // TESTENVIRONMENT_H
