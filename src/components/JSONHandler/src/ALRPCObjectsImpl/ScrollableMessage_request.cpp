#include "../../include/JSONHandler/ALRPCObjects/ScrollableMessage_request.h"
#include "ScrollableMessage_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "SoftButtonMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


ScrollableMessage_request::~ScrollableMessage_request(void)
{
}


ScrollableMessage_request::ScrollableMessage_request(const ScrollableMessage_request& c)
{
  *this=c;
}


bool ScrollableMessage_request::checkIntegrity(void)
{
  return ScrollableMessage_requestMarshaller::checkIntegrity(*this);
}


ScrollableMessage_request::ScrollableMessage_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SCROLLABLEMESSAGE_REQUEST)
{
}



bool ScrollableMessage_request::set_scrollableMessageBody(const std::string& scrollableMessageBody_)
{
  if(scrollableMessageBody_.length()>500)  return false;
  scrollableMessageBody=scrollableMessageBody_;
  return true;
}

bool ScrollableMessage_request::set_timeout(unsigned int timeout_)
{
  if(timeout_>65535)  return false;
  timeout=timeout_;
  return true;
}

bool ScrollableMessage_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>8 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  softButtons=softButtons_;
  return true;
}




const std::string& ScrollableMessage_request::get_scrollableMessageBody(void) const 
{
  return scrollableMessageBody;
}

unsigned int ScrollableMessage_request::get_timeout(void) const
{
  return timeout;
}

const std::vector<SoftButton>& ScrollableMessage_request::get_softButtons(void) const 
{
  return softButtons;
}

