#include "../../include/JSONHandler/ALRPCObjects/EncodedSyncPData_request.h"
#include "EncodedSyncPData_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

EncodedSyncPData_request& EncodedSyncPData_request::operator =(const EncodedSyncPData_request& c)
{
  data= c.data ? new std::vector<std::string>(c.data[0]) : 0;

  return *this;}


EncodedSyncPData_request::~EncodedSyncPData_request(void)
{
  if(data)
    delete data;
}


EncodedSyncPData_request::EncodedSyncPData_request(const EncodedSyncPData_request& c)
{
  *this=c;
}


bool EncodedSyncPData_request::checkIntegrity(void)
{
  return EncodedSyncPData_requestMarshaller::checkIntegrity(*this);
}


EncodedSyncPData_request::EncodedSyncPData_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ENCODEDSYNCPDATA_REQUEST),
      data(0)
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
  delete data;
  data=0;

  data=new std::vector<std::string>(data_);
  return true;
}

void EncodedSyncPData_request::reset_data(void)
{
  if(data)
    delete data;
  data=0;
}




const std::vector<std::string>* EncodedSyncPData_request::get_data(void) const 
{
  return data;
}

