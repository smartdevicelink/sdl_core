#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/UpdateTurnList.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


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
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__UPDATETURNLIST)
{
}


UpdateTurnList::UpdateTurnList(const UpdateTurnList& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__UPDATETURNLIST,c.getId())
{
  *this=c;
}


const std::vector< NsSmartDeviceLinkRPCV2::Turn>& UpdateTurnList::get_turnList(void)
{
  return turnList;
}

bool UpdateTurnList::set_turnList(const std::vector< NsSmartDeviceLinkRPCV2::Turn>& turnList_)
{
  turnList=turnList_;
  return true;
}

const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& UpdateTurnList::get_softButtons(void)
{
  return softButtons;
}

bool UpdateTurnList::set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons_)
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
