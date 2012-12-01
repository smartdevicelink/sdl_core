#include "../include/JSONHandler/ALRPCObjects/EncodedSyncPData_v2_request.h"
#include "EncodedSyncPData_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

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
EncodedSyncPData_v2_request& EncodedSyncPData_v2_request::operator =(const EncodedSyncPData_v2_request& c)
{
  data= c.data ? new std::vector<std::string>(c.data[0]) : 0;

  return *this;}


EncodedSyncPData_v2_request::~EncodedSyncPData_v2_request(void)
{
  if(data)
    delete data;
}


EncodedSyncPData_v2_request::EncodedSyncPData_v2_request(const EncodedSyncPData_v2_request& c)
{
  *this=c;
}


bool EncodedSyncPData_v2_request::checkIntegrity(void)
{
  return EncodedSyncPData_v2_requestMarshaller::checkIntegrity(*this);
}


EncodedSyncPData_v2_request::EncodedSyncPData_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ENCODEDSYNCPDATA_V2_REQUEST),
      data(0)
{
}



bool EncodedSyncPData_v2_request::set_data(const std::vector<std::string>& data_)
{
  unsigned int i=data_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(data_[i].length()>10000)  return false;
  }
  delete data;
  data=0;

  data=new std::vector<std::string>(data_);
  return true;
}

void EncodedSyncPData_v2_request::reset_data(void)
{
  if(data)
    delete data;
  data=0;
}




const std::vector<std::string>* EncodedSyncPData_v2_request::get_data(void) const 
{
  return data;
}

