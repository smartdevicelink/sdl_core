#include "../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered_v2.h"
#include "OnAppInterfaceUnregistered_v2Marshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "AppInterfaceUnregisteredReason_v2Marshaller.h"

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

OnAppInterfaceUnregistered_v2::~OnAppInterfaceUnregistered_v2(void)
{
}


OnAppInterfaceUnregistered_v2::OnAppInterfaceUnregistered_v2(const OnAppInterfaceUnregistered_v2& c)
{
  *this=c;
}


bool OnAppInterfaceUnregistered_v2::checkIntegrity(void)
{
  return OnAppInterfaceUnregistered_v2Marshaller::checkIntegrity(*this);
}


OnAppInterfaceUnregistered_v2::OnAppInterfaceUnregistered_v2(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONAPPINTERFACEUNREGISTERED_V2)
{
}



bool OnAppInterfaceUnregistered_v2::set_reason(const AppInterfaceUnregisteredReason_v2& reason_)
{
  if(!AppInterfaceUnregisteredReason_v2Marshaller::checkIntegrityConst(reason_))   return false;
  reason=reason_;
  return true;
}




const AppInterfaceUnregisteredReason_v2& OnAppInterfaceUnregistered_v2::get_reason(void) const 
{
  return reason;
}

