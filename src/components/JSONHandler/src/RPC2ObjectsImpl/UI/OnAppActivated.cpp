#include "../include/JSONHandler/RPC2Objects/UI/OnAppActivated.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace RPC2Communication::UI;


OnAppActivated& OnAppActivated::operator =(const OnAppActivated& c)
{
  appName=c.appName;
  return *this;
}


OnAppActivated::~OnAppActivated(void)
{
}


OnAppActivated::OnAppActivated(void) : 
  RPC2Notification(Marshaller::METHOD_ONAPPACTIVATED)
{
}


OnAppActivated::OnAppActivated(const OnAppActivated& c) : RPC2Notification(Marshaller::METHOD_ONAPPACTIVATED)
{
  *this=c;
}


const std::string& OnAppActivated::get_appName(void)
{
  return appName;
}

bool OnAppActivated::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

bool OnAppActivated::checkIntegrity(void)
{
  return OnAppActivatedMarshaller::checkIntegrity(*this);
}
