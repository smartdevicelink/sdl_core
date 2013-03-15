//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetDisplayLayoutResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


SetDisplayLayoutResponse& SetDisplayLayoutResponse::operator =(const SetDisplayLayoutResponse& c)
{
  return *this;
}


SetDisplayLayoutResponse::~SetDisplayLayoutResponse(void)
{
}


SetDisplayLayoutResponse::SetDisplayLayoutResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUTRESPONSE)
{
}


SetDisplayLayoutResponse::SetDisplayLayoutResponse(const SetDisplayLayoutResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool SetDisplayLayoutResponse::checkIntegrity(void)
{
  return SetDisplayLayoutResponseMarshaller::checkIntegrity(*this);
}
