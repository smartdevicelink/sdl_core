#include "../include/JSONHandler/ALRPCObjects/UpdateTurnList_request.h"
#include "UpdateTurnList_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "SoftButtonMarshaller.h"
#include "TurnMarshaller.h"

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

UpdateTurnList_request::~UpdateTurnList_request(void)
{
}


UpdateTurnList_request::UpdateTurnList_request(const UpdateTurnList_request& c)
{
  *this=c;
}


bool UpdateTurnList_request::checkIntegrity(void)
{
  return UpdateTurnList_requestMarshaller::checkIntegrity(*this);
}


UpdateTurnList_request::UpdateTurnList_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_UPDATETURNLIST_REQUEST)
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

