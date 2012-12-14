#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/UpdateTurnList.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;


UpdateTurnList& UpdateTurnList::operator =(const UpdateTurnList& c)
{
  turnList=c.turnList;
  softButtons=c.softButtons;
  appId=c.appId;
  return *this;
}


UpdateTurnList::~UpdateTurnList(void)
{
}


UpdateTurnList::UpdateTurnList(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLIST)
{
}


UpdateTurnList::UpdateTurnList(const UpdateTurnList& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_NAVIGATION__UPDATETURNLIST,c.getId())
{
  *this=c;
}


const std::vector< NsAppLinkRPCV2::Turn>& UpdateTurnList::get_turnList(void)
{
  return turnList;
}

bool UpdateTurnList::set_turnList(const std::vector< NsAppLinkRPCV2::Turn>& turnList_)
{
  turnList=turnList_;
  return true;
}

const std::vector< NsAppLinkRPCV2::SoftButton>& UpdateTurnList::get_softButtons(void)
{
  return softButtons;
}

bool UpdateTurnList::set_softButtons(const std::vector< NsAppLinkRPCV2::SoftButton>& softButtons_)
{
  softButtons=softButtons_;
  return true;
}

int UpdateTurnList::get_appId(void)
{
  return appId;
}

bool UpdateTurnList::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool UpdateTurnList::checkIntegrity(void)
{
  return UpdateTurnListMarshaller::checkIntegrity(*this);
}
