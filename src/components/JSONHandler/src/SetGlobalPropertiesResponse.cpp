#include "JSONHandler/SetGlobalPropertiesResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

SetGlobalPropertiesResponse::SetGlobalPropertiesResponse() 
: RPC2Response( RPC2Marshaller::METHOD_SET_GLOBAL_PROPERTIES_RESPONSE ) 
{}
        
SetGlobalPropertiesResponse::~SetGlobalPropertiesResponse() 
{}