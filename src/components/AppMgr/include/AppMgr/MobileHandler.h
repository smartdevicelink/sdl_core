#ifndef MOBILEHANDLER_H
#define MOBILEHANDLER_H

class JSONHandler;

namespace log4cplus
{
class Logger;
}

namespace NsAppLinkRPC
{
class ALRPCMessage;
}

namespace NsAppManager
{

/**
 * \brief MobileHandler acts as wrapper for mobile JSON handler
 */
class MobileHandler
{
public:

    /**
     * \brief Returning class instance
     * \return class instance
     */
    static MobileHandler &getInstance();

    /**
     * \brief send a message via associated handler
     * \param message message to send
     * \param sessionId id of a session associated with application that sent the message
     */
    void sendRPCMessage( const NsAppLinkRPC::ALRPCMessage * message, unsigned char sessionId );

    /**
     * \brief set mobile Json handler
     * \param handler mobile Json handler instance
     */
    void setJsonHandler(JSONHandler* handler);

    /**
     * \brief get mobile Json handler
     * \return mobile Json handler instance
     */
    JSONHandler* getJsonHandler( ) const;

private:

    /**
     * \brief Default class constructor
     */
    MobileHandler();

    /**
     * \brief Copy constructor
     */
    MobileHandler(const MobileHandler&);

    JSONHandler* mJSONHandler;

    static log4cplus::Logger mLogger;
};

}

#endif // MOBILEHANDLER_H
