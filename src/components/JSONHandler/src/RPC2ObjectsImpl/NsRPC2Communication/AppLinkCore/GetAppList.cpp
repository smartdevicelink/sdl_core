#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetAppList.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


GetAppList& GetAppList::operator =(const GetAppList& c)
{
  return *this;
}


GetAppList::~GetAppList(void)
{
}


GetAppList::GetAppList(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST)
{
}


GetAppList::GetAppList(const GetAppList& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST,c.getId())
{
  *this=c;
}


bool GetAppList::checkIntegrity(void)
{
  return GetAppListMarshaller::checkIntegrity(*this);
}
