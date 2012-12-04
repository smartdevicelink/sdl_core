#include "../include/JSONHandler/ALRPCObjects/V1/DeleteSubMenu_request.h"
#include "DeleteSubMenu_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

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

