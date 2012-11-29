#include "../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered.h"
#include "OnAppInterfaceUnregisteredMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "AppInterfaceUnregisteredReasonMarshaller.h"

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

OnAppInterfaceUnregistered::~OnAppInterfaceUnregistered(void)
{
}


OnAppInterfaceUnregistered::OnAppInterfaceUnregistered(const OnAppInterfaceUnregistered& c)
{
  *this=c;
}


bool OnAppInterfaceUnregistered::checkIntegrity(void)
{
  return OnAppInterfaceUnregisteredMarshaller::checkIntegrity(*this);
}


OnAppInterfaceUnregistered::OnAppInterfaceUnregistered(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONAPPINTERFACEUNREGISTERED)
{
}



bool OnAppInterfaceUnregistered::set_reason(const AppInterfaceUnregisteredReason& reason_)
{
  if(!AppInterfaceUnregisteredReasonMarshaller::checkIntegrityConst(reason_))   return false;
  reason=reason_;
  return true;
}




const AppInterfaceUnregisteredReason& OnAppInterfaceUnregistered::get_reason(void) const 
{
  return reason;
}

