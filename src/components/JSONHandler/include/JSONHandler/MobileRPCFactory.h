#ifndef MOBILERPCFACTORY_CLASS
#define MOBILERPCFACTORY_CLASS value

#include "JSONHandler/RegisterAppInterface.h"
#include "JSONHandler/RegisterAppInterfaceResponse.h"
#include "JSONHandler/OnHMIStatus.h"
#include "JSONHandler/SubscribeButton.h"
#include "JSONHandler/SubscribeButtonResponse.h"
#include "JSONHandler/UnsubscribeButton.h"
#include "JSONHandler/UnsubscribeButtonResponse.h"
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

    RegisterAppInterface createRegisterAppInterface ( const MobileRPCMessage & message ) const;
    Json::Value serializeRegisterAppInterface( const RegisterAppInterface & request ) const;

    RegisterAppInterfaceResponse* createRegisterAppInterfaceResponse ( const MobileRPCMessage & message ) const;
    Json::Value serializeRegisterAppInterfaceResponse( const RegisterAppInterfaceResponse & response ) const;

    SubscribeButton createSubscribeButton ( const MobileRPCMessage & message ) const;
    Json::Value serializeSubscribeButton( const SubscribeButton & request ) const;

    SubscribeButtonResponse* createSubscribeButtonResponse ( const MobileRPCMessage & message ) const;
    Json::Value serializeSubscribeButtonResponse( const SubscribeButtonResponse & response ) const;

    OnHMIStatus * createOnHMIStatus () const;
    Json::Value serializeOnHMIStatus( const OnHMIStatus & notification ) const;

    RegisterAppInterface createRegisterAppInterface ( const Json::Value & jsonValue );

    /* data */
};

#endif
