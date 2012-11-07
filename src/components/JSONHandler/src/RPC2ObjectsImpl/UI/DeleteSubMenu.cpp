#include "../include/JSONHandler/RPC2Objects/UI/DeleteSubMenu.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace RPC2Communication::UI;


DeleteSubMenu& DeleteSubMenu::operator =(const DeleteSubMenu& c)
{
  menuId=c.menuId;
  return *this;
}


DeleteSubMenu::~DeleteSubMenu(void)
{
}


DeleteSubMenu::DeleteSubMenu(void) : 
  RPC2Request(Marshaller::METHOD_DELETESUBMENU)
{
}


DeleteSubMenu::DeleteSubMenu(const DeleteSubMenu& c) : RPC2Request(Marshaller::METHOD_DELETESUBMENU,c.getId())
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
