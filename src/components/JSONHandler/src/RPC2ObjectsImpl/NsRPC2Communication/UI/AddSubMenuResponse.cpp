//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


AddSubMenuResponse& AddSubMenuResponse::operator =(const AddSubMenuResponse& c)
{
  return *this;
}


AddSubMenuResponse::~AddSubMenuResponse(void)
{
}


AddSubMenuResponse::AddSubMenuResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE)
{
}


AddSubMenuResponse::AddSubMenuResponse(const AddSubMenuResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AddSubMenuResponse::checkIntegrity(void)
{
  return AddSubMenuResponseMarshaller::checkIntegrity(*this);
}
