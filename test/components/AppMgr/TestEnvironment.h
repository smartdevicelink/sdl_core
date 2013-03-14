#ifndef TESTENVIRONMENT_H
#define TESTENVIRONMENT_H

#include "mobileEmulator/ALRPC.h"

namespace NsTest
{
    typedef void (*OnMobileMessageReceived)(const NsSmartDeviceLinkRPC::SDLRPCMessage*, int, unsigned char);

    /**
     * \brief The TestEnvironment class acts as a main spot of sending/receiving mobile RPC messages
     */
    class TestEnvironment : public NsMobileEmulator::ALRPC
    {
    public:

        /**
         * \brief Constructor
         */
        TestEnvironment();

        /**
         * \brief send a message to HMI via applinkcore
         * \param message Pointer to base class of AppLink Json object
         * \param connectionId ID of the connection the message was received within.
         * \param sessionId ID of the session the message was received within.
         */
        void sendRPCMessageToHmi(NsSmartDeviceLinkRPC::SDLRPCMessage *message, int connectionId, unsigned char sessionId );

        /**
         * \brief Gets invoked upon a mobile RPC message gets received from HMI
         * \param message Pointer to base class of AppLink Json object
         * \param connectionId ID of the connection the message was received within.
         * \param sessionId ID of the session the message was received within.
         */
        virtual void messageReceivedFromHmiCallback( const NsSmartDeviceLinkRPC::SDLRPCMessage * message, int connectionId, unsigned char sessionId );

        /**
         * \brief Register a callback function to be called upon a new mobile message from HMI arrival
         * \param cbFn callback function
         */
        void registerMobileMessageReceivedCallback(OnMobileMessageReceived cbFn);

    private:

        /**
         * \brief CopyConstructor
         */
        TestEnvironment(const TestEnvironment&);

        OnMobileMessageReceived mOnMobileMessageReceivedCallback;
    };

}

#endif // TESTENVIRONMENT_H
