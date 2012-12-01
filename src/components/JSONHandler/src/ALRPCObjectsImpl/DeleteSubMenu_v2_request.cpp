#include "../include/JSONHandler/ALRPCObjects/DeleteSubMenu_v2_request.h"
#include "DeleteSubMenu_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

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

DeleteSubMenu_v2_request::~DeleteSubMenu_v2_request(void)
{
}


DeleteSubMenu_v2_request::DeleteSubMenu_v2_request(const DeleteSubMenu_v2_request& c)
{
  *this=c;
}


bool DeleteSubMenu_v2_request::checkIntegrity(void)
{
  return DeleteSubMenu_v2_requestMarshaller::checkIntegrity(*this);
}


DeleteSubMenu_v2_request::DeleteSubMenu_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_DELETESUBMENU_V2_REQUEST)
{
}



bool DeleteSubMenu_v2_request::set_menuID(unsigned int menuID_)
{
  if(menuID_>2000000000)  return false;
  menuID=menuID_;
  return true;
}




unsigned int DeleteSubMenu_v2_request::get_menuID(void) const
{
  return menuID;
}

