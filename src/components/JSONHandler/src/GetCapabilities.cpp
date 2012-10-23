#include "JSONHandler/GetCapabilities.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

GetCapabilities::GetCapabilities()
:RPC2Request( RPC2Marshaller::METHOD_GET_CAPABILITIES_REQUEST )
{}
 
GetCapabilities::~GetCapabilities()
{}