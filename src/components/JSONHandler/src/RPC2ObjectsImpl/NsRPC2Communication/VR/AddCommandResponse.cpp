//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/AddCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VR;


AddCommandResponse& AddCommandResponse::operator =(const AddCommandResponse& c)
{
  return *this;
}


AddCommandResponse::~AddCommandResponse(void)
{
}


AddCommandResponse::AddCommandResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE)
{
}


AddCommandResponse::AddCommandResponse(const AddCommandResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AddCommandResponse::checkIntegrity(void)
{
  return AddCommandResponseMarshaller::checkIntegrity(*this);
}
