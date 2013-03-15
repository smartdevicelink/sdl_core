//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnAppDeactivated.h"
#include "../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;


OnAppDeactivated& OnAppDeactivated::operator =(const OnAppDeactivated& c)
{
  appName=c.appName;
  reason=c.reason;
  appId=c.appId;
  return *this;
}


OnAppDeactivated::~OnAppDeactivated(void)
{
}


OnAppDeactivated::OnAppDeactivated(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__ONAPPDEACTIVATED)
{
}


OnAppDeactivated::OnAppDeactivated(const OnAppDeactivated& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_BASICCOMMUNICATION__ONAPPDEACTIVATED,c.getId())
{
  *this=c;
}


const std::string& OnAppDeactivated::get_appName(void)
{
  return appName;
}

bool OnAppDeactivated::set_appName(const std::string& appName_)
{
  appName=appName_;
  return true;
}

const NsSmartDeviceLinkRPCV2::DeactivateReason& OnAppDeactivated::get_reason(void)
{
  return reason;
}

bool OnAppDeactivated::set_reason(const NsSmartDeviceLinkRPCV2::DeactivateReason& reason_)
{
  reason=reason_;
  return true;
}

int OnAppDeactivated::get_appId(void)
{
  return appId;
}

bool OnAppDeactivated::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool OnAppDeactivated::checkIntegrity(void)
{
  return OnAppDeactivatedMarshaller::checkIntegrity(*this);
}
