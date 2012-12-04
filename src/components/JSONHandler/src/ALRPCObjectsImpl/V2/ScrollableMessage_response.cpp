#include "../include/JSONHandler/ALRPCObjects/V2/ScrollableMessage_response.h"
#include "ScrollableMessage_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

ScrollableMessage_response::~ScrollableMessage_response(void)
{
}


ScrollableMessage_response::ScrollableMessage_response(const ScrollableMessage_response& c) : ALRPC2Message(c)
{
  *this=c;
}


bool ScrollableMessage_response::checkIntegrity(void)
{
  return ScrollableMessage_responseMarshaller::checkIntegrity(*this);
}


ScrollableMessage_response::ScrollableMessage_response(void) : ALRPC2Message(PROTOCOL_VERSION)
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

