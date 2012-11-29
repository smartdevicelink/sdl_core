#include "../include/JSONHandler/ALRPCObjects/ReadDID_request.h"
#include "ReadDID_requestMarshaller.h"
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
ReadDID_request& ReadDID_request::operator =(const ReadDID_request& c)
{
  ecuName= c.ecuName ? new unsigned int(c.ecuName[0]) : 0;
  didLocation= c.didLocation ? new std::vector<unsigned int>(c.didLocation[0]) : 0;
  encrypted= c.encrypted ? new bool(c.encrypted[0]) : 0;

  return *this;}


ReadDID_request::~ReadDID_request(void)
{
  if(ecuName)
    delete ecuName;
  if(didLocation)
    delete didLocation;
  if(encrypted)
    delete encrypted;
}


ReadDID_request::ReadDID_request(const ReadDID_request& c)
{
  *this=c;
}


bool ReadDID_request::checkIntegrity(void)
{
  return ReadDID_requestMarshaller::checkIntegrity(*this);
}


ReadDID_request::ReadDID_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_READDID_REQUEST),
      ecuName(0),
    didLocation(0),
    encrypted(0)
{
}



bool ReadDID_request::set_ecuName(unsigned int ecuName_)
{
  if(ecuName_>65535)  return false;
  delete ecuName;
  ecuName=0;

  ecuName=new unsigned int(ecuName_);
  return true;
}

void ReadDID_request::reset_ecuName(void)
{
  if(ecuName)
    delete ecuName;
  ecuName=0;
}

bool ReadDID_request::set_didLocation(const std::vector<unsigned int>& didLocation_)
{
  unsigned int i=didLocation_.size();
  if(i>1000 || i<0)  return false;
  while(i--)
  {
    if(didLocation_[i]>65535)  return false;
  }
  delete didLocation;
  didLocation=0;

  didLocation=new std::vector<unsigned int>(didLocation_);
  return true;
}

void ReadDID_request::reset_didLocation(void)
{
  if(didLocation)
    delete didLocation;
  didLocation=0;
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




const unsigned int* ReadDID_request::get_ecuName(void) const 
{
  return ecuName;
}

const std::vector<unsigned int>* ReadDID_request::get_didLocation(void) const 
{
  return didLocation;
}

const bool* ReadDID_request::get_encrypted(void) const 
{
  return encrypted;
}

