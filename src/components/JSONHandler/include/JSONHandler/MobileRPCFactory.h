#ifndef MOBILERPCFACTORY_CLASS
#define MOBILERPCFACTORY_CLASS value

#include "JSONHandler/RegisterAppInterface.h"
#include <json/value.h>

class MobileRPCResponse;
class MobileRPCRequest;
class MobileRPCNotification;

class MobileRPCFactory
{
public:
    MobileRPCFactory();
    virtual ~MobileRPCFactory();

    MobileRPCResponse * createResponse ( const MobileRPCRequest & request );
    MobileRPCResponse * createResponse ( );
    
    MobileRPCNotification * createNotification ( const MobileRPCRequest & request );
    MobileRPCNotification * createNotification ( );

    RegisterAppInterface createRegisterAppInterface ( const MobileRPCMessage & message );
    Json::Value serializeRegisterAppInterface( const RegisterAppInterface & request );

    RegisterAppInterface createRegisterAppInterface ( const Json::Value & jsonValue );

    /* data */
};

#endif