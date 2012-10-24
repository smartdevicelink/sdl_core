#include "../../include/JSONHandler/ALRPCObjects/ScrollableMessage_response.h"
#include "ScrollableMessage_responseMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

bool ScrollableMessage_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}




bool ScrollableMessage_response::get_success(void) const
{
  return success;
}

const Result& ScrollableMessage_response::get_resultCode(void) const 
{
  return resultCode;
}

