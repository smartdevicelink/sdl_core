#include "../include/JSONHandler/RPC2Objects/UI/ResetGlobalProperties.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


ResetGlobalProperties& ResetGlobalProperties::operator =(const ResetGlobalProperties& c)
{
  properties=c.properties;
  return *this;
}


ResetGlobalProperties::~ResetGlobalProperties(void)
{
}


ResetGlobalProperties::ResetGlobalProperties(void) : 
  RPC2Request(Marshaller::METHOD_RESETGLOBALPROPERTIES)
{
}


ResetGlobalProperties::ResetGlobalProperties(const ResetGlobalProperties& c) : RPC2Request(Marshaller::METHOD_RESETGLOBALPROPERTIES,c.getId())
{
  *this=c;
}


const std::vector< GlobalProperty>& ResetGlobalProperties::get_properties(void)
{
  return properties;
}

bool ResetGlobalProperties::set_properties(const std::vector< GlobalProperty>& properties_)
{
  properties=properties_;
  return true;
}

bool ResetGlobalProperties::checkIntegrity(void)
{
  return ResetGlobalPropertiesMarshaller::checkIntegrity(*this);
}
