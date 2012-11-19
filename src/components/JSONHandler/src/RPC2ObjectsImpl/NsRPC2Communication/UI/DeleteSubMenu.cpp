#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteSubMenu.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


DeleteSubMenu& DeleteSubMenu::operator =(const DeleteSubMenu& c)
{
  menuId=c.menuId;
  return *this;
}


DeleteSubMenu::~DeleteSubMenu(void)
{
}


DeleteSubMenu::DeleteSubMenu(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU)
{
}


DeleteSubMenu::DeleteSubMenu(const DeleteSubMenu& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENU,c.getId())
{
  *this=c;
}


unsigned int DeleteSubMenu::get_menuId(void)
{
  return menuId;
}

bool DeleteSubMenu::set_menuId(unsigned int menuId_)
{
  menuId=menuId_;
  return true;
}

bool DeleteSubMenu::checkIntegrity(void)
{
  return DeleteSubMenuMarshaller::checkIntegrity(*this);
}
