#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/DeactivateApp.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


DeactivateApp& DeactivateApp::operator =(const DeactivateApp& c)
{
  appName=c.appName;
  reason=c.reason;
  appId=c.appId;
  return *this;
}


DeactivateApp::~DeactivateApp(void)
{
}


DeactivateApp::DeactivateApp(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPP)
{
}


DeactivateApp::DeactivateApp(const DeactivateApp& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__DEACTIVATEAPP,c.getId())
{
  *this=c;
}


const std::string& DeactivateApp::get_appName(void)
{
  return appName;
}

bool DeactivateApp::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

const NsAppLinkRPC::DeactivateReason& DeactivateApp::get_reason(void)
{
  return reason;
}

bool DeactivateApp::set_reason(const NsAppLinkRPC::DeactivateReason& reason_)
{
  reason=reason_;
  return true;
}

int DeactivateApp::get_appId(void)
{
  return appId;
}

bool DeactivateApp::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool DeactivateApp::checkIntegrity(void)
{
  return DeactivateAppMarshaller::checkIntegrity(*this);
}
