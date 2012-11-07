#include "../include/JSONHandler/RPC2Objects/VR/GetCapabilities.h"
#include "../include/JSONHandler/RPC2Objects/VR/Marshaller.h"

/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace RPC2Communication::VR;


GetCapabilities& GetCapabilities::operator =(const GetCapabilities& c)
{
  return *this;
}


GetCapabilities::~GetCapabilities(void)
{
}


GetCapabilities::GetCapabilities(void) : 
  RPC2Request(Marshaller::METHOD_GETCAPABILITIES)
{
}


GetCapabilities::GetCapabilities(const GetCapabilities& c) : RPC2Request(Marshaller::METHOD_GETCAPABILITIES,c.getId())
{
  *this=c;
}


bool GetCapabilities::checkIntegrity(void)
{
  return GetCapabilitiesMarshaller::checkIntegrity(*this);
}
