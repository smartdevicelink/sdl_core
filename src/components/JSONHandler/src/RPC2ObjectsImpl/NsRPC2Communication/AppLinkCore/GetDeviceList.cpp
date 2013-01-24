#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetDeviceList.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


GetDeviceList& GetDeviceList::operator =(const GetDeviceList& c)
{
  return *this;
}


GetDeviceList::~GetDeviceList(void)
{
}


GetDeviceList::GetDeviceList(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELIST)
{
}


GetDeviceList::GetDeviceList(const GetDeviceList& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELIST,c.getId())
{
  *this=c;
}


bool GetDeviceList::checkIntegrity(void)
{
  return GetDeviceListMarshaller::checkIntegrity(*this);
}
