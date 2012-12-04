#include "../include/JSONHandler/ALRPCObjects/V2/OnAppInterfaceUnregistered.h"
#include "OnAppInterfaceUnregisteredMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "AppInterfaceUnregisteredReasonMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnAppInterfaceUnregistered::~OnAppInterfaceUnregistered(void)
{
}


OnAppInterfaceUnregistered::OnAppInterfaceUnregistered(const OnAppInterfaceUnregistered& c) : ALRPC2Message(c)
{
  *this=c;
}


bool OnAppInterfaceUnregistered::checkIntegrity(void)
{
  return OnAppInterfaceUnregisteredMarshaller::checkIntegrity(*this);
}


OnAppInterfaceUnregistered::OnAppInterfaceUnregistered(void) : ALRPC2Message(PROTOCOL_VERSION)
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

