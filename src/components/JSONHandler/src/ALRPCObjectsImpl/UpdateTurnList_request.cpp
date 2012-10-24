#include "../../include/JSONHandler/ALRPCObjects/UpdateTurnList_request.h"
#include "UpdateTurnList_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "SoftButtonMarshaller.h"
#include "TurnMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

