#include "TestEnvironment.h"

namespace NsTest
{
    /**
     * \brief Constructor
     */
    TestEnvironment::TestEnvironment()
    {
    }

    /**
     * \brief send a message to HMI via applinkcore
     * \param message Pointer to base class of AppLink Json object
     * \param connectionId ID of the connection the message was received within.
     * \param sessionId ID of the session the message was received within.
     */
    void TestEnvironment::sendRPCMessageToHmi(NsSmartDeviceLinkRPC::SDLRPCMessage *message, int connectionId, unsigned char sessionId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Sending to HMI a message " << message->getMethodId() << " connection " << connectionId << " session " << (uint)sessionId);
        getRPCMessageObserver()->onMessageReceivedCallback( message, connectionId, sessionId );
    }

    /**
     * \brief Gets invoked upon a mobile RPC message gets received from HMI
     * \param message Pointer to base class of AppLink Json object
     * \param connectionId ID of the connection the message was received within.
     * \param sessionId ID of the session the message was received within.
     */
    void TestEnvironment::messageReceivedFromHmiCallback(const NsSmartDeviceLinkRPC::SDLRPCMessage *message, int connectionId, unsigned char sessionId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Received from HMI a message " << message->getMethodId() << " connection " << connectionId << " session " << (uint)sessionId);
        if(mOnMobileMessageReceivedCallback)
        {
            LOG4CPLUS_INFO_EXT(mLogger, " Calling a callback function...");
            mOnMobileMessageReceivedCallback(message, connectionId, sessionId);
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Callback function not set!");
        }
    }

    /**
     * \brief Register a callback function to be called upon a new mobile message from HMI arrival
     * \param cbFn callback function
     */
    void TestEnvironment::registerMobileMessageReceivedCallback(OnMobileMessageReceived cbFn)
    {
        mOnMobileMessageReceivedCallback = cbFn;
    }

    /**
     * \brief CopyConstructor
     */
    TestEnvironment::TestEnvironment(const TestEnvironment &)
    {
    }

}
