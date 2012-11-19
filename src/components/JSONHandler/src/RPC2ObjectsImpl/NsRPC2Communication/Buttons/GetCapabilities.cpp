#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Mon Nov 19 12:18:27 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Buttons;


GetCapabilities& GetCapabilities::operator =(const GetCapabilities& c)
{
  return *this;
}


GetCapabilities::~GetCapabilities(void)
{
}


GetCapabilities::GetCapabilities(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES)
{
}


GetCapabilities::GetCapabilities(const GetCapabilities& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIES,c.getId())
{
  *this=c;
}


bool GetCapabilities::checkIntegrity(void)
{
  return GetCapabilitiesMarshaller::checkIntegrity(*this);
}
