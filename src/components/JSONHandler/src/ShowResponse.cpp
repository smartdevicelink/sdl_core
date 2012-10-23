#include "JSONHandler/ShowResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

ShowResponse::ShowResponse() 
: RPC2Response( RPC2Marshaller::METHOD_SHOW_RESPONSE ) 
{}

ShowResponse::~ShowResponse() 
{}