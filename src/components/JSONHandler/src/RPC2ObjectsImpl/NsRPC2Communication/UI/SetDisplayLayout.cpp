#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetDisplayLayout.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


SetDisplayLayout& SetDisplayLayout::operator =(const SetDisplayLayout& c)
{
  displayLayout=c.displayLayout;
  appId=c.appId;
  return *this;
}


SetDisplayLayout::~SetDisplayLayout(void)
{
}


SetDisplayLayout::SetDisplayLayout(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUT)
{
}


SetDisplayLayout::SetDisplayLayout(const SetDisplayLayout& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETDISPLAYLAYOUT,c.getId())
{
  *this=c;
}


const std::string& SetDisplayLayout::get_displayLayout(void)
{
  return displayLayout;
}

bool SetDisplayLayout::set_displayLayout(const std::string& displayLayout_)
{
  displayLayout=displayLayout_;
  return true;
}

int SetDisplayLayout::get_appId(void)
{
  return appId;
}

bool SetDisplayLayout::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool SetDisplayLayout::checkIntegrity(void)
{
  return SetDisplayLayoutMarshaller::checkIntegrity(*this);
}
