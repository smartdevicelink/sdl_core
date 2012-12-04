#include "../include/JSONHandler/ALRPCObjects/V2/UpdateTurnList_request.h"
#include "UpdateTurnList_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "SoftButtonMarshaller.h"
#include "TurnMarshaller.h"

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

UpdateTurnList_request::~UpdateTurnList_request(void)
{
}


UpdateTurnList_request::UpdateTurnList_request(const UpdateTurnList_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool UpdateTurnList_request::checkIntegrity(void)
{
  return UpdateTurnList_requestMarshaller::checkIntegrity(*this);
}


UpdateTurnList_request::UpdateTurnList_request(void) : ALRPC2Message(PROTOCOL_VERSION)
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

