#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/GetAppListResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
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


const std::vector< NsAppLinkRPCV2::HMIApplication>& GetAppListResponse::get_appList(void)
{
  return appList;
}

bool GetAppListResponse::set_appList(const std::vector< NsAppLinkRPCV2::HMIApplication>& appList_)
{
  appList=appList_;
  return true;
}

bool GetAppListResponse::checkIntegrity(void)
{
  return GetAppListResponseMarshaller::checkIntegrity(*this);
}
