//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


DeleteSubMenuResponse& DeleteSubMenuResponse::operator =(const DeleteSubMenuResponse& c)
{
  return *this;
}


DeleteSubMenuResponse::~DeleteSubMenuResponse(void)
{
}


DeleteSubMenuResponse::DeleteSubMenuResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE)
{
}


DeleteSubMenuResponse::DeleteSubMenuResponse(const DeleteSubMenuResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool DeleteSubMenuResponse::checkIntegrity(void)
{
  return DeleteSubMenuResponseMarshaller::checkIntegrity(*this);
}
