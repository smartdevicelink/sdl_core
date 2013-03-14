#include "../include/JSONHandler/SDLRPCObjects/V2/ReadDID_request.h"
#include "ReadDID_requestMarshaller.h"
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
ReadDID_request& ReadDID_request::operator =(const ReadDID_request& c)
{
  ecuName= c.ecuName;
  didLocation= c.didLocation;
  encrypted= c.encrypted ? new bool(c.encrypted[0]) : 0;

  return *this;
}


ReadDID_request::~ReadDID_request(void)
{
  if(encrypted)
    delete encrypted;
}


ReadDID_request::ReadDID_request(const ReadDID_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool ReadDID_request::checkIntegrity(void)
{
  return ReadDID_requestMarshaller::checkIntegrity(*this);
}


ReadDID_request::ReadDID_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION),
      encrypted(0)
{
}



bool ReadDID_request::set_ecuName(unsigned int ecuName_)
{
  if(ecuName_>65535)  return false;
  ecuName=ecuName_;
  return true;
}

bool ReadDID_request::set_didLocation(const std::vector<unsigned int>& didLocation_)
{
  unsigned int i=didLocation_.size();
  if(i>1000 || i<0)  return false;
  while(i--)
  {
    if(didLocation_[i]>65535)  return false;
  }
  didLocation=didLocation_;
  return true;
}

bool ReadDID_request::set_encrypted(bool encrypted_)
{
  delete encrypted;
  encrypted=0;

  encrypted=new bool(encrypted_);
  return true;
}

void ReadDID_request::reset_encrypted(void)
{
  if(encrypted)
    delete encrypted;
  encrypted=0;
}




unsigned int ReadDID_request::get_ecuName(void) const
{
  return ecuName;
}

const std::vector<unsigned int>& ReadDID_request::get_didLocation(void) const 
{
  return didLocation;
}

const bool* ReadDID_request::get_encrypted(void) const 
{
  return encrypted;
}

