#include "../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_v2_request.h"
#include "ResetGlobalProperties_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "GlobalProperty_v2Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

ResetGlobalProperties_v2_request::~ResetGlobalProperties_v2_request(void)
{
}


ResetGlobalProperties_v2_request::ResetGlobalProperties_v2_request(const ResetGlobalProperties_v2_request& c)
{
  *this=c;
}


bool ResetGlobalProperties_v2_request::checkIntegrity(void)
{
  return ResetGlobalProperties_v2_requestMarshaller::checkIntegrity(*this);
}


ResetGlobalProperties_v2_request::ResetGlobalProperties_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_RESETGLOBALPROPERTIES_V2_REQUEST)
{
}



bool ResetGlobalProperties_v2_request::set_properties(const std::vector<GlobalProperty_v2>& properties_)
{
  unsigned int i=properties_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!GlobalProperty_v2Marshaller::checkIntegrityConst(properties_[i]))   return false;
  }
  properties=properties_;
  return true;
}




const std::vector<GlobalProperty_v2>& ResetGlobalProperties_v2_request::get_properties(void) const 
{
  return properties;
}

