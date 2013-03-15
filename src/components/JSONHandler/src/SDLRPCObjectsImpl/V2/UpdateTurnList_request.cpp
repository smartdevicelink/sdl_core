//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/UpdateTurnList_request.h"
#include "UpdateTurnList_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "SoftButtonMarshaller.h"
#include "TurnMarshaller.h"

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

UpdateTurnList_request::~UpdateTurnList_request(void)
{
}


UpdateTurnList_request::UpdateTurnList_request(const UpdateTurnList_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool UpdateTurnList_request::checkIntegrity(void)
{
  return UpdateTurnList_requestMarshaller::checkIntegrity(*this);
}


UpdateTurnList_request::UpdateTurnList_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}



bool UpdateTurnList_request::set_turnList(const std::vector<Turn>& turnList_)
{
  unsigned int i=turnList_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TurnMarshaller::checkIntegrityConst(turnList_[i]))   return false;
  }
  turnList=turnList_;
  return true;
}

bool UpdateTurnList_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>1 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  softButtons=softButtons_;
  return true;
}




const std::vector<Turn>& UpdateTurnList_request::get_turnList(void) const 
{
  return turnList;
}

const std::vector<SoftButton>& UpdateTurnList_request::get_softButtons(void) const 
{
  return softButtons;
}

