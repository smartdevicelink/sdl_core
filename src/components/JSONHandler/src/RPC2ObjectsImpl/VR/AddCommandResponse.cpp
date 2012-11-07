#include "../include/JSONHandler/RPC2Objects/VR/AddCommandResponse.h"
#include "../include/JSONHandler/RPC2Objects/VR/Marshaller.h"

/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::VR;


AddCommandResponse& AddCommandResponse::operator =(const AddCommandResponse& c)
{
  return *this;
}


AddCommandResponse::~AddCommandResponse(void)
{
}


AddCommandResponse::AddCommandResponse(void) : 
  RPC2Response(Marshaller::METHOD_ADDCOMMANDRESPONSE)
{
}


AddCommandResponse::AddCommandResponse(const AddCommandResponse& c) : RPC2Response(Marshaller::METHOD_ADDCOMMANDRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


bool AddCommandResponse::checkIntegrity(void)
{
  return AddCommandResponseMarshaller::checkIntegrity(*this);
}
