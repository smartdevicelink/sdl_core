#include "../include/JSONHandler/ALRPCObjects/MenuParams.h"
#include "MenuParamsMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



using namespace AppLinkRPC;

MenuParams& MenuParams::operator =(const MenuParams& c)
{
  menuName=c.menuName;
  parentID=c.parentID ? new unsigned int(c.parentID[0]) : 0;
  position=c.position ? new unsigned int(c.position[0]) : 0;

  return *this;
}


MenuParams::~MenuParams(void)
{
  if(parentID)
    delete parentID;
  if(position)
    delete position;
}


MenuParams::MenuParams(const MenuParams& c)
{
  *this=c;
}


bool MenuParams::checkIntegrity(void)
{
  return MenuParamsMarshaller::checkIntegrity(*this);
}


MenuParams::MenuParams(void) :
    parentID(0),
    position(0)
{
}



bool MenuParams::set_menuName(const std::string& menuName_)
{
  if(menuName_.length()>500)  return false;
  menuName=menuName_;
  return true;
}

bool MenuParams::set_parentID(unsigned int parentID_)
{
  if(parentID_>2000000000)  return false;
  delete parentID;
  parentID=0;

  parentID=new unsigned int(parentID_);
  return true;
}

void MenuParams::reset_parentID(void)
{
  if(parentID)
    delete parentID;
  parentID=0;
}

bool MenuParams::set_position(unsigned int position_)
{
  if(position_>1000)  return false;
  delete position;
  position=0;

  position=new unsigned int(position_);
  return true;
}

void MenuParams::reset_position(void)
{
  if(position)
    delete position;
  position=0;
}




const std::string& MenuParams::get_menuName(void) const 
{
  return menuName;
}


const unsigned int* MenuParams::get_parentID(void) const 
{
  return parentID;
}


const unsigned int* MenuParams::get_position(void) const 
{
  return position;
}


