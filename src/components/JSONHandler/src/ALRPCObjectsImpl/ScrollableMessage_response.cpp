#include "../include/JSONHandler/ALRPCObjects/ScrollableMessage_response.h"
#include "ScrollableMessage_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "Result_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

ScrollableMessage_response::~ScrollableMessage_response(void)
{
}


ScrollableMessage_response::ScrollableMessage_response(const ScrollableMessage_response& c)
{
  *this=c;
}


bool ScrollableMessage_response::checkIntegrity(void)
{
  return ScrollableMessage_responseMarshaller::checkIntegrity(*this);
}


ScrollableMessage_response::ScrollableMessage_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_SCROLLABLEMESSAGE_RESPONSE)
{
}



bool ScrollableMessage_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool ScrollableMessage_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}




bool ScrollableMessage_response::get_success(void) const
{
  return success;
}

const Result_v2& ScrollableMessage_response::get_resultCode(void) const 
{
  return resultCode;
}

