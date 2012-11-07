#include "../include/JSONHandler/ALRPCObjects/DeleteSubMenu_request.h"
#include "DeleteSubMenu_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace AppLinkRPC;

DeleteSubMenu_request::~DeleteSubMenu_request(void)
{
}


DeleteSubMenu_request::DeleteSubMenu_request(const DeleteSubMenu_request& c)
{
  *this=c;
}


bool DeleteSubMenu_request::checkIntegrity(void)
{
  return DeleteSubMenu_requestMarshaller::checkIntegrity(*this);
}


DeleteSubMenu_request::DeleteSubMenu_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_DELETESUBMENU_REQUEST)
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

