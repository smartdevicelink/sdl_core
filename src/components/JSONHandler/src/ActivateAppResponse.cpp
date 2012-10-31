#include "JSONHandler/ActivateAppResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

ActivateAppResponse::ActivateAppResponse() 
: RPC2Response( RPC2Marshaller::METHOD_ACTIVATEAPP_RESPONSE ) 
{}
        
ActivateAppResponse::~ActivateAppResponse() 
{}