#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
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


const std::vector< NsAppLinkRPC::GlobalProperty>& ResetGlobalProperties::get_properties(void)
{
  return properties;
}

bool ResetGlobalProperties::set_properties(const std::vector< NsAppLinkRPC::GlobalProperty>& properties_)
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
