#include "JSONHandler/AlertResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

AlertResponse::AlertResponse() 
: RPC2Response( RPC2Marshaller::METHOD_ALERT_RESPONSE ) 
{}
        
AlertResponse::~AlertResponse() 
{}
