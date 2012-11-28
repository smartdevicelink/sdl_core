#include "ALRPC.h"

namespace NsMobileEmulator
{
    log4cplus::Logger ALRPC::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("testAppMgr"));

    /**
    * \brief Constructor.
    */
    ALRPC::ALRPC()
    {
    }

    /**
    * \brief Destructor.
    */
    ALRPC::~ALRPC()
    {
    }

    /**
     * \brief Sets pointer to instance of the class implementing RPC handling (Application Manager).
     * \param messagesObserver Pointer to object of the class implementing IRPCMessagesObserver
     * \sa IRPCMessagesObserver
     */
    void ALRPC::setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver )
    {
        if ( !messagesObserver )
        {
            LOG4CPLUS_ERROR_EXT( mLogger, "Invalid (null) pointer to IRPCMessagesObserver." );
        }
        mMessagesObserver = messagesObserver;
    }

    /**
     * \brief Method for sending message to Mobile Application.
     * \param message Pointer to base class of AppLink Json object
     * to be serialized to Json message and sent to mobile App.
     * \param connectionId ID of the connection the message was received within.
     * \param sessionId ID of the session the message was received within.
     */
    void ALRPC::sendRPCMessage(const NsAppLinkRPC::ALRPCMessage *message, int connectionId, unsigned char sessionId)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Sending an RPC message " << message->getMethodId() << " connection " << connectionId << " session " << sessionId);
        messageReceivedFromHmiCallback(message, connectionId, sessionId);
    }

    /**
    * \brief Get message observer instance
    * \return message observer instance
    */
    IRPCMessagesObserver *ALRPC::getRPCMessageObserver() const
    {
        if ( !mMessagesObserver )
        {
            LOG4CPLUS_ERROR_EXT( mLogger, "Invalid (null) pointer to IRPCMessagesObserver retreived!" );
        }
        return mMessagesObserver;
    }

}
