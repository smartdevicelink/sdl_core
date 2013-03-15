//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DialNumberResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


DialNumberResponse& DialNumberResponse::operator =(const DialNumberResponse& c)
{
  return *this;
}


DialNumberResponse::~DialNumberResponse(void)
{
}


DialNumberResponse::DialNumberResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DIALNUMBERRESPONSE)
{
}


DialNumberResponse::DialNumberResponse(const DialNumberResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DIALNUMBERRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DialNumberResponse::checkIntegrity(void)
{
  return DialNumberResponseMarshaller::checkIntegrity(*this);
}
