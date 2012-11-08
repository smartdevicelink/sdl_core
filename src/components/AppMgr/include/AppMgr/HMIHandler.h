#ifndef HMIHANDLER_H
#define HMIHANDLER_H

class JSONRPC2Handler;

namespace RPC2Communication
{
class RPC2Notification;
class RPC2Response;
class RPC2Request;
}

namespace NsAppManager
{

class HMIHandler
{
public:
    static HMIHandler &getInstance();

    void setReadyState(bool ready);

    void sendNotification( const RPC2Communication::RPC2Notification * command );
    void sendResponse( const RPC2Communication::RPC2Response * command );
    void sendRequest( const RPC2Communication::RPC2Request * command );

    void setJsonRPC2Handler(JSONRPC2Handler* handler);
    JSONRPC2Handler* getJsonRPC2Handler( ) const;

private:
    HMIHandler();
    HMIHandler(const HMIHandler&);
    bool             m_bHMIReady;
    JSONRPC2Handler* mJSONRPC2Handler;
};

}

#endif // HMIHANDLER_H
