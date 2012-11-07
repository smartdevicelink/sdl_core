#include "../include/JSONHandler/RPC2Objects/AppLinkCore/ActivateApp.h"
#include "../include/JSONHandler/RPC2Objects/AppLinkCore/Marshaller.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:25:43 2012
  source stamp	Wed Nov  7 09:35:35 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace RPC2Communication::AppLinkCore;


ActivateApp& ActivateApp::operator =(const ActivateApp& c)
{
  appName=c.appName;
  return *this;
}


ActivateApp::~ActivateApp(void)
{
}


ActivateApp::ActivateApp(void) : 
  RPC2Request(Marshaller::METHOD_ACTIVATEAPP)
{
}


ActivateApp::ActivateApp(const ActivateApp& c) : RPC2Request(Marshaller::METHOD_ACTIVATEAPP,c.getId())
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

bool ActivateApp::checkIntegrity(void)
{
  return ActivateAppMarshaller::checkIntegrity(*this);
}
