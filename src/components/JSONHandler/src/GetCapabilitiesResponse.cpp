#include "JSONHandler/GetCapabilitiesResponse.h"
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

GetCapabilitiesResponse::GetCapabilitiesResponse()
:RPC2Response( RPC2Marshaller::METHOD_GET_CAPABILITIES_RESPONSE )
{}
 
GetCapabilitiesResponse::~GetCapabilitiesResponse()
{}

void GetCapabilitiesResponse::setCapabilities(const std::vector<GetCapabilitiesResponse::GetCapabilitiesResponseInternal> &v)
{
    mCapabilities = v;
}
    
std::vector<GetCapabilitiesResponse::GetCapabilitiesResponseInternal> GetCapabilitiesResponse::getCapabilities() const
{
    return mCapabilities;
}
