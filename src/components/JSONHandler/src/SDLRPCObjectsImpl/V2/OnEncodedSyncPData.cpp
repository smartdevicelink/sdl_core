#include "../include/JSONHandler/SDLRPCObjects/V2/OnEncodedSyncPData.h"
#include "OnEncodedSyncPDataMarshaller.h"
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
OnEncodedSyncPData& OnEncodedSyncPData::operator =(const OnEncodedSyncPData& c)
{
  data= c.data;
  URL= c.URL ? new std::string(c.URL[0]) : 0;
  Timeout= c.Timeout ? new int(c.Timeout[0]) : 0;

  return *this;
}


OnEncodedSyncPData::~OnEncodedSyncPData(void)
{
  if(URL)
    delete URL;
  if(Timeout)
    delete Timeout;
}


OnEncodedSyncPData::OnEncodedSyncPData(const OnEncodedSyncPData& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool OnEncodedSyncPData::checkIntegrity(void)
{
  return OnEncodedSyncPDataMarshaller::checkIntegrity(*this);
}


OnEncodedSyncPData::OnEncodedSyncPData(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      URL(0),
    Timeout(0)
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
  data=data_;
  return true;
}

bool OnEncodedSyncPData::set_URL(const std::string& URL_)
{
  if(URL_.length()>1000)  return false;
  delete URL;
  URL=0;

  URL=new std::string(URL_);
  return true;
}

void OnEncodedSyncPData::reset_URL(void)
{
  if(URL)
    delete URL;
  URL=0;
}

bool OnEncodedSyncPData::set_Timeout(int Timeout_)
{
  if(Timeout_>2000000000)  return false;
  delete Timeout;
  Timeout=0;

  Timeout=new int(Timeout_);
  return true;
}

void OnEncodedSyncPData::reset_Timeout(void)
{
  if(Timeout)
    delete Timeout;
  Timeout=0;
}




const std::vector<std::string>& OnEncodedSyncPData::get_data(void) const 
{
  return data;
}

const std::string* OnEncodedSyncPData::get_URL(void) const 
{
  return URL;
}

const int* OnEncodedSyncPData::get_Timeout(void) const 
{
  return Timeout;
}

