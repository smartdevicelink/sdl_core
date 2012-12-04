#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/DeactivateApp.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


DeactivateApp& DeactivateApp::operator =(const DeactivateApp& c)
{
  appName=c.appName;
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
