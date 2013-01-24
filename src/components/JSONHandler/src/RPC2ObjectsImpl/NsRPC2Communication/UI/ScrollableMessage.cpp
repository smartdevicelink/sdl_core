#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ScrollableMessage.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ScrollableMessage& ScrollableMessage::operator =(const ScrollableMessage& c)
{
  scrollableMessageBody=c.scrollableMessageBody;
  if(timeout)  delete timeout;
  timeout= c.timeout ? new unsigned int(c.timeout[0]) : 0;
  if(softButtons)  delete softButtons;
  softButtons= c.softButtons ? new std::vector<NsAppLinkRPCV2::SoftButton>(c.softButtons[0]) : 0;
  appId=c.appId;
  return *this;
}


ScrollableMessage::~ScrollableMessage(void)
{
  if(timeout)  delete timeout;
  if(softButtons)  delete softButtons;
}


ScrollableMessage::ScrollableMessage(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGE),
  timeout(0),
  softButtons(0)
{
}


ScrollableMessage::ScrollableMessage(const ScrollableMessage& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SCROLLABLEMESSAGE,c.getId())
{
  *this=c;
}


const std::string& ScrollableMessage::get_scrollableMessageBody(void)
{
  return scrollableMessageBody;
}

bool ScrollableMessage::set_scrollableMessageBody(const std::string& scrollableMessageBody_)
{
  scrollableMessageBody=scrollableMessageBody_;
  return true;
}

const unsigned int* ScrollableMessage::get_timeout(void)
{
  return timeout;
}

bool ScrollableMessage::set_timeout(const unsigned int& timeout_)
{
  if(timeout)  delete timeout;
  timeout=new unsigned int(timeout_);
  return true;
}

void ScrollableMessage::reset_timeout(void)
{
  if(timeout)  delete timeout;
  timeout=0;
}

const std::vector< NsAppLinkRPCV2::SoftButton>* ScrollableMessage::get_softButtons(void)
{
  return softButtons;
}

bool ScrollableMessage::set_softButtons(const std::vector< NsAppLinkRPCV2::SoftButton>& softButtons_)
{
  if(softButtons)  delete softButtons;
  softButtons=new std::vector< NsAppLinkRPCV2::SoftButton>(softButtons_);
  return true;
}

void ScrollableMessage::reset_softButtons(void)
{
  if(softButtons)  delete softButtons;
  softButtons=0;
}

int ScrollableMessage::get_appId(void)
{
  return appId;
}

bool ScrollableMessage::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ScrollableMessage::checkIntegrity(void)
{
  return ScrollableMessageMarshaller::checkIntegrity(*this);
}
