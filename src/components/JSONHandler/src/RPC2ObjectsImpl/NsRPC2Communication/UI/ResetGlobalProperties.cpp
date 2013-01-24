#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


ResetGlobalProperties& ResetGlobalProperties::operator =(const ResetGlobalProperties& c)
{
  properties=c.properties;
  appId=c.appId;
  return *this;
}


ResetGlobalProperties::~ResetGlobalProperties(void)
{
}


ResetGlobalProperties::ResetGlobalProperties(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES)
{
}


ResetGlobalProperties::ResetGlobalProperties(const ResetGlobalProperties& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIES,c.getId())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::GlobalProperty>& ResetGlobalProperties::get_properties(void)
{
  return properties;
}

bool ResetGlobalProperties::set_properties(const std::vector< NsAppLinkRPCV2::GlobalProperty>& properties_)
{
  properties=properties_;
  return true;
}

int ResetGlobalProperties::get_appId(void)
{
  return appId;
}

bool ResetGlobalProperties::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ResetGlobalProperties::checkIntegrity(void)
{
  return ResetGlobalPropertiesMarshaller::checkIntegrity(*this);
}
