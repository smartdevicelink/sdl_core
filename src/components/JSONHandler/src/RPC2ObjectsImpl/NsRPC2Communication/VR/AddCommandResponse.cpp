#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/AddCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
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
