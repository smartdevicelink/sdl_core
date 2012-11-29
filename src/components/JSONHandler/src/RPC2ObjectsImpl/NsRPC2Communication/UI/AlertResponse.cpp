#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AlertResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


AlertResponse& AlertResponse::operator =(const AlertResponse& c)
{
  if(tryAgainTime)  delete tryAgainTime;
  tryAgainTime= c.tryAgainTime ? new unsigned int(c.tryAgainTime[0]) : 0;
  return *this;
}


AlertResponse::~AlertResponse(void)
{
  if(tryAgainTime)  delete tryAgainTime;
}


AlertResponse::AlertResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE),
  tryAgainTime(0)
{
}


AlertResponse::AlertResponse(const AlertResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const unsigned int* AlertResponse::get_tryAgainTime(void)
{
  return tryAgainTime;
}

bool AlertResponse::set_tryAgainTime(const unsigned int& tryAgainTime_)
{
  if(tryAgainTime)  delete tryAgainTime;
  tryAgainTime=new unsigned int(tryAgainTime_);
  return true;
}

void AlertResponse::reset_tryAgainTime(void)
{
  if(tryAgainTime)  delete tryAgainTime;
  tryAgainTime=0;
}

bool AlertResponse::checkIntegrity(void)
{
  return AlertResponseMarshaller::checkIntegrity(*this);
}
