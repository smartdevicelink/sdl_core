#include "../include/JSONHandler/RPC2Objects/UI/GetCapabilitiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


GetCapabilitiesResponse& GetCapabilitiesResponse::operator =(const GetCapabilitiesResponse& c)
{
  displayCapabilities=c.displayCapabilities;
  hmiZoneCapabilities=c.hmiZoneCapabilities;
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


const DisplayCapabilities& GetCapabilitiesResponse::get_displayCapabilities(void)
{
  return displayCapabilities;
}

bool GetCapabilitiesResponse::set_displayCapabilities(const DisplayCapabilities& displayCapabilities_)
{
  displayCapabilities=displayCapabilities_;
  return true;
}

const std::vector< HmiZoneCapabilities>& GetCapabilitiesResponse::get_hmiZoneCapabilities(void)
{
  return hmiZoneCapabilities;
}

bool GetCapabilitiesResponse::set_hmiZoneCapabilities(const std::vector< HmiZoneCapabilities>& hmiZoneCapabilities_)
{
  hmiZoneCapabilities=hmiZoneCapabilities_;
  return true;
}

bool GetCapabilitiesResponse::checkIntegrity(void)
{
  return GetCapabilitiesResponseMarshaller::checkIntegrity(*this);
}
