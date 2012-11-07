#include "../include/JSONHandler/RPC2Objects/UI/OnAppDeactivated.h"
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


OnAppDeactivated& OnAppDeactivated::operator =(const OnAppDeactivated& c)
{
  appName=c.appName;
  return *this;
}


OnAppDeactivated::~OnAppDeactivated(void)
{
}


OnAppDeactivated::OnAppDeactivated(void) : 
  RPC2Notification(Marshaller::METHOD_ONAPPDEACTIVATED)
{
}


OnAppDeactivated::OnAppDeactivated(const OnAppDeactivated& c) : RPC2Notification(Marshaller::METHOD_ONAPPDEACTIVATED)
{
  *this=c;
}


const std::string& OnAppDeactivated::get_appName(void)
{
  return appName;
}

bool OnAppDeactivated::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

bool OnAppDeactivated::checkIntegrity(void)
{
  return OnAppDeactivatedMarshaller::checkIntegrity(*this);
}
