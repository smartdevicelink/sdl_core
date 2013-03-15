//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/OnCommand.h"
#include "OnCommandMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "TriggerSourceMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;

OnCommand::~OnCommand(void)
{
}


OnCommand::OnCommand(const OnCommand& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool OnCommand::checkIntegrity(void)
{
  return OnCommandMarshaller::checkIntegrity(*this);
}


OnCommand::OnCommand(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
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

