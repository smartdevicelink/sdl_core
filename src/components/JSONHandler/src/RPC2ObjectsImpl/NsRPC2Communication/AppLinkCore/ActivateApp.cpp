#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/ActivateApp.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


ActivateApp& ActivateApp::operator =(const ActivateApp& c)
{
  appName=c.appName;
  appId=c.appId;
  return *this;
}


ActivateApp::~ActivateApp(void)
{
}


ActivateApp::ActivateApp(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPP)
{
}


ActivateApp::ActivateApp(const ActivateApp& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPP,c.getId())
{
  *this=c;
}


const std::string& ActivateApp::get_appName(void)
{
  return appName;
}

bool ActivateApp::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

int ActivateApp::get_appId(void)
{
  return appId;
}

bool ActivateApp::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ActivateApp::checkIntegrity(void)
{
  return ActivateAppMarshaller::checkIntegrity(*this);
}
