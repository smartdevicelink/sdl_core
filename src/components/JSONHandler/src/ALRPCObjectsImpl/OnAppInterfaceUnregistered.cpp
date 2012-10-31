#include "../../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered.h"
#include "OnAppInterfaceUnregisteredMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "AppInterfaceUnregisteredReasonMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


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

