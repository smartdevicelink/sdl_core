#include "../include/JSONHandler/RPC2Objects/UI/AddSubMenu.h"
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


AddSubMenu& AddSubMenu::operator =(const AddSubMenu& c)
{
  menuId=c.menuId;
  if(position)  delete position;
  position= c.position ? new unsigned int(c.position[0]) : 0;
  menuName=c.menuName;
  return *this;
}


AddSubMenu::~AddSubMenu(void)
{
  if(position)  delete position;
}


AddSubMenu::AddSubMenu(void) : 
  RPC2Request(Marshaller::METHOD_ADDSUBMENU),
  position(0)
{
}


AddSubMenu::AddSubMenu(const AddSubMenu& c) : RPC2Request(Marshaller::METHOD_ADDSUBMENU,c.getId())
{
  *this=c;
}


unsigned int AddSubMenu::get_menuId(void)
{
  return menuId;
}

bool AddSubMenu::set_menuId(unsigned int menuId_)
{
  menuId=menuId_;
  return true;
}

const unsigned int* AddSubMenu::get_position(void)
{
  return position;
}

bool AddSubMenu::set_position(const unsigned int& position_)
{
  if(position)  delete position;
  position=new unsigned int(position_);
  return true;
}

void AddSubMenu::reset_position(void)
{
  if(position)  delete position;
  position=0;
}

const std::string& AddSubMenu::get_menuName(void)
{
  return menuName;
}

bool AddSubMenu::set_menuName(const std::string& menuName_)
{
  menuName=menuName_;
  return true;
}

bool AddSubMenu::checkIntegrity(void)
{
  return AddSubMenuMarshaller::checkIntegrity(*this);
}
