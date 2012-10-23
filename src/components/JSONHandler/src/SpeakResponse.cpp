#include "JSONHandler/SpeakResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

SpeakResponse::SpeakResponse()
:RPC2Response( RPC2Marshaller::METHOD_SPEAK_RESPONSE )
{}
 
SpeakResponse::~SpeakResponse()
{}
