//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/ScrollableMessage_request.h"
#include "ScrollableMessage_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "SoftButtonMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;
ScrollableMessage_request& ScrollableMessage_request::operator =(const ScrollableMessage_request& c)
{
  scrollableMessageBody= c.scrollableMessageBody;
  timeout= c.timeout ? new unsigned int(c.timeout[0]) : 0;
  softButtons= c.softButtons ? new std::vector<SoftButton>(c.softButtons[0]) : 0;

  return *this;
}


ScrollableMessage_request::~ScrollableMessage_request(void)
{
  if(timeout)
    delete timeout;
  if(softButtons)
    delete softButtons;
}


ScrollableMessage_request::ScrollableMessage_request(const ScrollableMessage_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool ScrollableMessage_request::checkIntegrity(void)
{
  return ScrollableMessage_requestMarshaller::checkIntegrity(*this);
}


ScrollableMessage_request::ScrollableMessage_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      timeout(0),
    softButtons(0)
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
  delete timeout;
  timeout=0;

  timeout=new unsigned int(timeout_);
  return true;
}

void ScrollableMessage_request::reset_timeout(void)
{
  if(timeout)
    delete timeout;
  timeout=0;
}

bool ScrollableMessage_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>8 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  delete softButtons;
  softButtons=0;

  softButtons=new std::vector<SoftButton>(softButtons_);
  return true;
}

void ScrollableMessage_request::reset_softButtons(void)
{
  if(softButtons)
    delete softButtons;
  softButtons=0;
}




const std::string& ScrollableMessage_request::get_scrollableMessageBody(void) const 
{
  return scrollableMessageBody;
}

const unsigned int* ScrollableMessage_request::get_timeout(void) const 
{
  return timeout;
}

const std::vector<SoftButton>* ScrollableMessage_request::get_softButtons(void) const 
{
  return softButtons;
}

