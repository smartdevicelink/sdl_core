#include "../include/JSONHandler/ALRPCObjects/V2/DeleteSubMenu_request.h"
#include "DeleteSubMenu_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

DeleteSubMenu_request::~DeleteSubMenu_request(void)
{
}


DeleteSubMenu_request::DeleteSubMenu_request(const DeleteSubMenu_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool DeleteSubMenu_request::checkIntegrity(void)
{
  return DeleteSubMenu_requestMarshaller::checkIntegrity(*this);
}


DeleteSubMenu_request::DeleteSubMenu_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool DeleteSubMenu_request::set_menuID(unsigned int menuID_)
{
  if(menuID_>2000000000)  return false;
  menuID=menuID_;
  return true;
}




unsigned int DeleteSubMenu_request::get_menuID(void) const
{
  return menuID;
}

