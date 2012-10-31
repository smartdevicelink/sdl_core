#include "JSONHandler/ResetGlobalPropertiesResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

ResetGlobalPropertiesResponse::ResetGlobalPropertiesResponse() 
: RPC2Response( RPC2Marshaller::METHOD_RESET_GLOBAL_PROPERTIES_RESPONSE ) 
{}
        
ResetGlobalPropertiesResponse::~ResetGlobalPropertiesResponse() 
{}