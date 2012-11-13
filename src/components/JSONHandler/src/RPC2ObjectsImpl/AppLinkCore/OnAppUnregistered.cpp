#include "../include/JSONHandler/RPC2Objects/AppLinkCore/OnAppUnregistered.h"
#include "../include/JSONHandler/RPC2Objects/AppLinkCore/Marshaller.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:25:43 2012
  source stamp	Wed Nov  7 09:35:35 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::AppLinkCore;


OnAppUnregistered& OnAppUnregistered::operator =(const OnAppUnregistered& c)
{
  appName=c.appName;
  if(reason)  delete reason;
  reason= c.reason ? new AppInterfaceUnregisteredReason(c.reason[0]) : 0;
  return *this;
}


OnAppUnregistered::~OnAppUnregistered(void)
{
  if(reason)  delete reason;
}


OnAppUnregistered::OnAppUnregistered(void) : 
  RPC2Notification(Marshaller::METHOD_ONAPPUNREGISTERED),
  reason(0)
{
}


OnAppUnregistered::OnAppUnregistered(const OnAppUnregistered& c) : RPC2Notification(Marshaller::METHOD_ONAPPUNREGISTERED)
{
  *this=c;
}


const std::string& OnAppUnregistered::get_appName(void)
{
  return appName;
}

bool OnAppUnregistered::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

const AppInterfaceUnregisteredReason* OnAppUnregistered::get_reason(void)
{
  return reason;
}

bool OnAppUnregistered::set_reason(const AppInterfaceUnregisteredReason& reason_)
{
  if(reason)  delete reason;
  reason=new AppInterfaceUnregisteredReason(reason_);
  return true;
}

void OnAppUnregistered::reset_reason(void)
{
  if(reason)  delete reason;
  reason=0;
}

bool OnAppUnregistered::checkIntegrity(void)
{
  return OnAppUnregisteredMarshaller::checkIntegrity(*this);
}
