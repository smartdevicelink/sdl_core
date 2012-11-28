#include "TestEnvironment.h"

namespace NsTest
{
    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
     * \param messagesObserver Pointer to object of the class implementing IRPCMessagesObserver
     * \sa IRPCMessagesObserver
     */
    void TestEnvironment::setRPCMessagesObserver(IRPCMessagesObserver *messagesObserver)
    {
        NsMobileEmulator::ALRPC::setRPCMessagesObserver(messagesObserver);
    }

    /**
     * \brief send a message to HMI via applinkcore
     * \param message Pointer to base class of AppLink Json object
     * \param connectionId ID of the connection the message was received within.
     * \param sessionId ID of the session the message was received within.
     */
    void TestEnvironment::sendToHmi(NsAppLinkRPC::ALRPCMessage *message, int connectionId, unsigned char sessionId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Sending to HMI a message " << message->getMethodId() << " connection " << connectionId << " session " << sessionId);
        getRPCMessageObserver()->onMessageReceivedCallback( message, connectionId, sessionId );
    }

    /**
     * \brief Gets invoked upon a mobile RPC message gets received from HMI
     * \param message Pointer to base class of AppLink Json object
     * \param connectionId ID of the connection the message was received within.
     * \param sessionId ID of the session the message was received within.
     */
    void TestEnvironment::messageReceivedFromHmiCallback(const NsAppLinkRPC::ALRPCMessage *message, int connectionId, unsigned char sessionId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Received from HMI a message " << message->getMethodId() << " connection " << connectionId << " session " << sessionId);
    }

}
