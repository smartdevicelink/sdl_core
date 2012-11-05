#include "JSONHandler/DeleteInteractionChoiceSetResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

DeleteInteractionChoiceSetResponse::DeleteInteractionChoiceSetResponse() 
: RPC2Response( RPC2Marshaller::METHOD_DELETEINTERACTIONCHOICESET_RESPONSE ) 
{}
        
DeleteInteractionChoiceSetResponse::~DeleteInteractionChoiceSetResponse() 
{}