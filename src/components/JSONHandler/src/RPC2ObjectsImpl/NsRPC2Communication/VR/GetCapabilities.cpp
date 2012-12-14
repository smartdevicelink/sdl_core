#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


GetCapabilities& GetCapabilities::operator =(const GetCapabilities& c)
{
  return *this;
}


GetCapabilities::~GetCapabilities(void)
{
}


GetCapabilities::GetCapabilities(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES)
{
}


GetCapabilities::GetCapabilities(const GetCapabilities& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIES,c.getId())
{
  *this=c;
}


bool GetCapabilities::checkIntegrity(void)
{
  return GetCapabilitiesMarshaller::checkIntegrity(*this);
}
