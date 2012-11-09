#ifndef MOBILEHANDLER_H
#define MOBILEHANDLER_H

class JSONHandler;

namespace log4cplus
{
class Logger;
}

namespace AppLinkRPC
{
class ALRPCMessage;
}

namespace NsAppManager
{

class MobileHandler
{
public:
    static MobileHandler &getInstance();

    void sendRPCMessage( const AppLinkRPC::ALRPCMessage * message, unsigned char sessionId );

    void setJsonHandler(JSONHandler* handler);
    JSONHandler* getJsonHandler( ) const;

private:
    MobileHandler();
    MobileHandler(const MobileHandler&);

    JSONHandler* mJSONHandler;

    static log4cplus::Logger mLogger;
};

}

#endif // MOBILEHANDLER_H
