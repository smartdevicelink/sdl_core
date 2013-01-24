#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


GetCapabilities& GetCapabilities::operator =(const GetCapabilities& c)
{
  return *this;
}


GetCapabilities::~GetCapabilities(void)
{
}


GetCapabilities::GetCapabilities(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES)
{
}


GetCapabilities::GetCapabilities(const GetCapabilities& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIES,c.getId())
{
  *this=c;
}


bool GetCapabilities::checkIntegrity(void)
{
  return GetCapabilitiesMarshaller::checkIntegrity(*this);
}
