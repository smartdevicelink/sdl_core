#include "../include/JSONHandler/RPC2Objects/VR/GetCapabilitiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/VR/Marshaller.h"

/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::VR;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  capabilities=c.capabilities;
  return *this;
}


GetCapabilitiesResponse::~GetCapabilitiesResponse(void)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(void) : 
  RPC2Response(Marshaller::METHOD_GETCAPABILITIESRESPONSE)
{
}


GetCapabilitiesResponse::GetCapabilitiesResponse(const GetCapabilitiesResponse& c) : RPC2Response(Marshaller::METHOD_GETCAPABILITIESRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< VrCapabilities>& GetCapabilitiesResponse::get_capabilities(void)
{
  return capabilities;
}

bool GetCapabilitiesResponse::set_capabilities(const std::vector< VrCapabilities>& capabilities_)
{
  capabilities=capabilities_;
  return true;
}

bool GetCapabilitiesResponse::checkIntegrity(void)
{
  return GetCapabilitiesResponseMarshaller::checkIntegrity(*this);
}
