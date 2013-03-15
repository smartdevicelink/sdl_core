//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetDTCs.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VehicleInfo
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::VehicleInfo;


GetDTCs& GetDTCs::operator =(const GetDTCs& c)
{
  ecuName=c.ecuName;
  if(encrypted)  delete encrypted;
  encrypted= c.encrypted ? new bool(c.encrypted[0]) : 0;
  appId=c.appId;
  return *this;
}


GetDTCs::~GetDTCs(void)
{
  if(encrypted)  delete encrypted;
}


GetDTCs::GetDTCs(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCS),
  encrypted(0)
{
}


GetDTCs::GetDTCs(const GetDTCs& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VEHICLEINFO__GETDTCS,c.getId())
{
  *this=c;
}


unsigned int GetDTCs::get_ecuName(void)
{
  return ecuName;
}

bool GetDTCs::set_ecuName(unsigned int ecuName_)
{
  ecuName=ecuName_;
  return true;
}

const bool* GetDTCs::get_encrypted(void)
{
  return encrypted;
}

bool GetDTCs::set_encrypted(const bool& encrypted_)
{
  if(encrypted)  delete encrypted;
  encrypted=new bool(encrypted_);
  return true;
}

void GetDTCs::reset_encrypted(void)
{
  if(encrypted)  delete encrypted;
  encrypted=0;
}

int GetDTCs::get_appId(void)
{
  return appId;
}

bool GetDTCs::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool GetDTCs::checkIntegrity(void)
{
  return GetDTCsMarshaller::checkIntegrity(*this);
}
