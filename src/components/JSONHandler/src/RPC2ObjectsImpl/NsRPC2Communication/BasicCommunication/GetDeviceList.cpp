//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/GetDeviceList.h"
#include "../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;


GetDeviceList& GetDeviceList::operator =(const GetDeviceList& c)
{
  return *this;
}


GetDeviceList::~GetDeviceList(void)
{
}


GetDeviceList::GetDeviceList(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__GETDEVICELIST)
{
}


GetDeviceList::GetDeviceList(const GetDeviceList& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__GETDEVICELIST,c.getId())
{
  *this=c;
}


bool GetDeviceList::checkIntegrity(void)
{
  return GetDeviceListMarshaller::checkIntegrity(*this);
}
