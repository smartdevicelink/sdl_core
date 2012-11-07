#include "../include/JSONHandler/ALRPCObjects/ResetGlobalProperties_request.h"
#include "ResetGlobalProperties_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "GlobalPropertyMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace AppLinkRPC;

ResetGlobalProperties_request::~ResetGlobalProperties_request(void)
{
}


ResetGlobalProperties_request::ResetGlobalProperties_request(const ResetGlobalProperties_request& c)
{
  *this=c;
}


bool ResetGlobalProperties_request::checkIntegrity(void)
{
  return ResetGlobalProperties_requestMarshaller::checkIntegrity(*this);
}


ResetGlobalProperties_request::ResetGlobalProperties_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_RESETGLOBALPROPERTIES_REQUEST)
{
}



bool ResetGlobalProperties_request::set_properties(const std::vector<GlobalProperty>& properties_)
{
  unsigned int i=properties_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!GlobalPropertyMarshaller::checkIntegrityConst(properties_[i]))   return false;
  }
  properties=properties_;
  return true;
}




const std::vector<GlobalProperty>& ResetGlobalProperties_request::get_properties(void) const 
{
  return properties;
}

