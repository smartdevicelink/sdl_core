#include "JSONHandler/CreateInteractionChoiceSetResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

CreateInteractionChoiceSetResponse::CreateInteractionChoiceSetResponse() 
: RPC2Response( RPC2Marshaller::METHOD_CREATEINTERACTIONCHOICESET_RESPONSE ) 
{}
        
CreateInteractionChoiceSetResponse::~CreateInteractionChoiceSetResponse() 
{}
