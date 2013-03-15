//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/ResetGlobalProperties_request.h"
#include "ResetGlobalProperties_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "GlobalPropertyMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;

ResetGlobalProperties_request::~ResetGlobalProperties_request(void)
{
}


ResetGlobalProperties_request::ResetGlobalProperties_request(const ResetGlobalProperties_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool ResetGlobalProperties_request::checkIntegrity(void)
{
  return ResetGlobalProperties_requestMarshaller::checkIntegrity(*this);
}


ResetGlobalProperties_request::ResetGlobalProperties_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
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

