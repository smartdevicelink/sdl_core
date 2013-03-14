#include "../include/JSONHandler/SDLRPCObjects/V2/EncodedSyncPData_request.h"
#include "EncodedSyncPData_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;

EncodedSyncPData_request::~EncodedSyncPData_request(void)
{
}


EncodedSyncPData_request::EncodedSyncPData_request(const EncodedSyncPData_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool EncodedSyncPData_request::checkIntegrity(void)
{
  return EncodedSyncPData_requestMarshaller::checkIntegrity(*this);
}


EncodedSyncPData_request::EncodedSyncPData_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}



bool EncodedSyncPData_request::set_data(const std::vector<std::string>& data_)
{
  unsigned int i=data_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(data_[i].length()>10000)  return false;
  }
  data=data_;
  return true;
}




const std::vector<std::string>& EncodedSyncPData_request::get_data(void) const 
{
  return data;
}

