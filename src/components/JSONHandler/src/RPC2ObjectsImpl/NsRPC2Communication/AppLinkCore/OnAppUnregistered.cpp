#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppUnregistered.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


OnAppUnregistered& OnAppUnregistered::operator =(const OnAppUnregistered& c)
{
  appName=c.appName;
  if(reason)  delete reason;
  reason= c.reason ? new NsAppLinkRPC::AppInterfaceUnregisteredReason(c.reason[0]) : 0;
  return *this;
}


OnAppUnregistered::~OnAppUnregistered(void)
{
  if(reason)  delete reason;
}


OnAppUnregistered::OnAppUnregistered(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED),
  reason(0)
{
}


OnAppUnregistered::OnAppUnregistered(const OnAppUnregistered& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED)
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

const NsAppLinkRPC::AppInterfaceUnregisteredReason* OnAppUnregistered::get_reason(void)
{
  return reason;
}

bool OnAppUnregistered::set_reason(const NsAppLinkRPC::AppInterfaceUnregisteredReason& reason_)
{
  if(reason)  delete reason;
  reason=new NsAppLinkRPC::AppInterfaceUnregisteredReason(reason_);
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
