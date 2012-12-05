#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetAppListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


GetAppListResponse& GetAppListResponse::operator =(const GetAppListResponse& c)
{
  appList=c.appList;
  return *this;
}


GetAppListResponse::~GetAppListResponse(void)
{
}


GetAppListResponse::GetAppListResponse(void) : 
  RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE)
{
}


GetAppListResponse::GetAppListResponse(const GetAppListResponse& c) : RPC2Response(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLISTRESPONSE,c.getId(),c.getResult())
{
  *this=c;
}


const std::vector< NsAppLinkRPC::HMIApplication>& GetAppListResponse::get_appList(void)
{
  return appList;
}

bool GetAppListResponse::set_appList(const std::vector< NsAppLinkRPC::HMIApplication>& appList_)
{
  appList=appList_;
  return true;
}

bool GetAppListResponse::checkIntegrity(void)
{
  return GetAppListResponseMarshaller::checkIntegrity(*this);
}
