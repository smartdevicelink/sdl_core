#include "JSONHandler/AddCommandResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

AddCommandResponse::AddCommandResponse() 
: RPC2Response( RPC2Marshaller::METHOD_ADDCOMMAND_RESPONSE ) 
{}
        
AddCommandResponse::~AddCommandResponse() 
{}