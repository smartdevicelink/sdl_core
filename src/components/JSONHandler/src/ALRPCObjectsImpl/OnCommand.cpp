#include "../include/JSONHandler/ALRPCObjects/OnCommand.h"
#include "OnCommandMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "TriggerSourceMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
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

