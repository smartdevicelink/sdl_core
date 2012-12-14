#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Phone/DialNumberResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Phone
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Phone;


DialNumberResponse& DialNumberResponse::operator =(const DialNumberResponse& c)
{
  return *this;
}


DialNumberResponse::~DialNumberResponse(void)
{
}


DialNumberResponse::DialNumberResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBERRESPONSE)
{
}


DialNumberResponse::DialNumberResponse(const DialNumberResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBERRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DialNumberResponse::checkIntegrity(void)
{
  return DialNumberResponseMarshaller::checkIntegrity(*this);
}
