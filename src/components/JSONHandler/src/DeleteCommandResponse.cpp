#include "JSONHandler/DeleteCommandResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

DeleteCommandResponse::DeleteCommandResponse() 
: RPC2Response( RPC2Marshaller::METHOD_DELETECOMMAND_RESPONSE ) 
{}
        
DeleteCommandResponse::~DeleteCommandResponse() 
{}