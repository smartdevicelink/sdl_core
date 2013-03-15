//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDID.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;


ReadDID& ReadDID::operator =(const ReadDID& c)
{
  ecuName=c.ecuName;
  didLocation=c.didLocation;
  if(encrypted)  delete encrypted;
  encrypted= c.encrypted ? new bool(c.encrypted[0]) : 0;
  appId=c.appId;
  return *this;
}


ReadDID::~ReadDID(void)
{
  if(encrypted)  delete encrypted;
}


ReadDID::ReadDID(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDID),
  encrypted(0)
{
}


ReadDID::ReadDID(const ReadDID& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__READDID,c.getId())
{
  *this=c;
}


unsigned int ReadDID::get_ecuName(void)
{
  return ecuName;
}

bool ReadDID::set_ecuName(unsigned int ecuName_)
{
  ecuName=ecuName_;
  return true;
}

const std::vector< unsigned int>& ReadDID::get_didLocation(void)
{
  return didLocation;
}

bool ReadDID::set_didLocation(const std::vector< unsigned int>& didLocation_)
{
  didLocation=didLocation_;
  return true;
}

const bool* ReadDID::get_encrypted(void)
{
  return encrypted;
}

bool ReadDID::set_encrypted(const bool& encrypted_)
{
  if(encrypted)  delete encrypted;
  encrypted=new bool(encrypted_);
  return true;
}

void ReadDID::reset_encrypted(void)
{
  if(encrypted)  delete encrypted;
  encrypted=0;
}

int ReadDID::get_appId(void)
{
  return appId;
}

bool ReadDID::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool ReadDID::checkIntegrity(void)
{
  return ReadDIDMarshaller::checkIntegrity(*this);
}
