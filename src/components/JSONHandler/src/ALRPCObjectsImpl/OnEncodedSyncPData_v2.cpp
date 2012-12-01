#include "../include/JSONHandler/ALRPCObjects/OnEncodedSyncPData_v2.h"
#include "OnEncodedSyncPData_v2Marshaller.h"
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
OnEncodedSyncPData_v2& OnEncodedSyncPData_v2::operator =(const OnEncodedSyncPData_v2& c)
{
  data= c.data ? new std::vector<std::string>(c.data[0]) : 0;
  URL= c.URL ? new std::string(c.URL[0]) : 0;
  Timeout= c.Timeout ? new int(c.Timeout[0]) : 0;

  return *this;}


OnEncodedSyncPData_v2::~OnEncodedSyncPData_v2(void)
{
  if(data)
    delete data;
  if(URL)
    delete URL;
  if(Timeout)
    delete Timeout;
}


OnEncodedSyncPData_v2::OnEncodedSyncPData_v2(const OnEncodedSyncPData_v2& c)
{
  *this=c;
}


bool OnEncodedSyncPData_v2::checkIntegrity(void)
{
  return OnEncodedSyncPData_v2Marshaller::checkIntegrity(*this);
}


OnEncodedSyncPData_v2::OnEncodedSyncPData_v2(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONENCODEDSYNCPDATA_V2),
      data(0),
    URL(0),
    Timeout(0)
{
}



bool OnEncodedSyncPData_v2::set_data(const std::vector<std::string>& data_)
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

void OnEncodedSyncPData_v2::reset_data(void)
{
  if(data)
    delete data;
  data=0;
}

bool OnEncodedSyncPData_v2::set_URL(const std::string& URL_)
{
  if(URL_.length()>1000)  return false;
  delete URL;
  URL=0;

  URL=new std::string(URL_);
  return true;
}

void OnEncodedSyncPData_v2::reset_URL(void)
{
  if(URL)
    delete URL;
  URL=0;
}

bool OnEncodedSyncPData_v2::set_Timeout(int Timeout_)
{
  if(Timeout_>2000000000)  return false;
  delete Timeout;
  Timeout=0;

  Timeout=new int(Timeout_);
  return true;
}

void OnEncodedSyncPData_v2::reset_Timeout(void)
{
  if(Timeout)
    delete Timeout;
  Timeout=0;
}




const std::vector<std::string>* OnEncodedSyncPData_v2::get_data(void) const 
{
  return data;
}

const std::string* OnEncodedSyncPData_v2::get_URL(void) const 
{
  return URL;
}

const int* OnEncodedSyncPData_v2::get_Timeout(void) const 
{
  return Timeout;
}

