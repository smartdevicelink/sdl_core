#include "../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData.h"
#include "OnEncodedSyncPDataMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

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
OnEncodedSyncPData& OnEncodedSyncPData::operator =(const OnEncodedSyncPData& c)
{
  data= c.data ? new std::vector<std::string>(c.data[0]) : 0;

  return *this;}


OnEncodedSyncPData::~OnEncodedSyncPData(void)
{
  if(data)
    delete data;
}


OnEncodedSyncPData::OnEncodedSyncPData(const OnEncodedSyncPData& c)
{
  *this=c;
}


bool OnEncodedSyncPData::checkIntegrity(void)
{
  return OnEncodedSyncPDataMarshaller::checkIntegrity(*this);
}


OnEncodedSyncPData::OnEncodedSyncPData(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONENCODEDSYNCPDATA),
      data(0)
{
}



bool OnEncodedSyncPData::set_data(const std::vector<std::string>& data_)
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

void OnEncodedSyncPData::reset_data(void)
{
  if(data)
    delete data;
  data=0;
}




const std::vector<std::string>* OnEncodedSyncPData::get_data(void) const 
{
  return data;
}

