//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V1/OnCommand.h"
#include "OnCommandMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V1/Marshaller.h"
#include "TriggerSourceMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPC;

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


OnCommand::OnCommand(void) : SDLRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONCOMMAND)
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

