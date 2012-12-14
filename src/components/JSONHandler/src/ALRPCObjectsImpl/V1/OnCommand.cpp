#include "../include/JSONHandler/ALRPCObjects/V1/OnCommand.h"
#include "OnCommandMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "TriggerSourceMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

OnCommand::~OnCommand(void)
{
}


OnCommand::OnCommand(const OnCommand& c)
{
  *this=c;
}


bool OnCommand::checkIntegrity(void)
{
  return OnCommandMarshaller::checkIntegrity(*this);
}


OnCommand::OnCommand(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONCOMMAND)
{
}



bool OnCommand::set_cmdID(unsigned int cmdID_)
{
  if(cmdID_>2000000000)  return false;
  cmdID=cmdID_;
  return true;
}

bool OnCommand::set_triggerSource(const TriggerSource& triggerSource_)
{
  if(!TriggerSourceMarshaller::checkIntegrityConst(triggerSource_))   return false;
  triggerSource=triggerSource_;
  return true;
}




unsigned int OnCommand::get_cmdID(void) const
{
  return cmdID;
}

const TriggerSource& OnCommand::get_triggerSource(void) const 
{
  return triggerSource;
}

