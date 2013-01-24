#include "../include/JSONHandler/ALRPCObjects/V2/GetDTCs_request.h"
#include "GetDTCs_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;
GetDTCs_request& GetDTCs_request::operator =(const GetDTCs_request& c)
{
  ecuName= c.ecuName;
  encrypted= c.encrypted ? new bool(c.encrypted[0]) : 0;

  return *this;
}


GetDTCs_request::~GetDTCs_request(void)
{
  if(encrypted)
    delete encrypted;
}


GetDTCs_request::GetDTCs_request(const GetDTCs_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool GetDTCs_request::checkIntegrity(void)
{
  return GetDTCs_requestMarshaller::checkIntegrity(*this);
}


GetDTCs_request::GetDTCs_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION),
      encrypted(0)
{
}



bool GetDTCs_request::set_ecuName(unsigned int ecuName_)
{
  if(ecuName_>65535)  return false;
  ecuName=ecuName_;
  return true;
}

bool GetDTCs_request::set_encrypted(bool encrypted_)
{
  delete encrypted;
  encrypted=0;

  encrypted=new bool(encrypted_);
  return true;
}

void GetDTCs_request::reset_encrypted(void)
{
  if(encrypted)
    delete encrypted;
  encrypted=0;
}




unsigned int GetDTCs_request::get_ecuName(void) const
{
  return ecuName;
}

const bool* GetDTCs_request::get_encrypted(void) const 
{
  return encrypted;
}

