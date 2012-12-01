#include "../include/JSONHandler/ALRPCObjects/AddSubMenu_v2_request.h"
#include "AddSubMenu_v2_requestMarshaller.h"
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
AddSubMenu_v2_request& AddSubMenu_v2_request::operator =(const AddSubMenu_v2_request& c)
{
  menuID= c.menuID;
  position= c.position ? new unsigned int(c.position[0]) : 0;
  menuName= c.menuName;

  return *this;}


AddSubMenu_v2_request::~AddSubMenu_v2_request(void)
{
  if(position)
    delete position;
}


AddSubMenu_v2_request::AddSubMenu_v2_request(const AddSubMenu_v2_request& c)
{
  *this=c;
}


bool AddSubMenu_v2_request::checkIntegrity(void)
{
  return AddSubMenu_v2_requestMarshaller::checkIntegrity(*this);
}


AddSubMenu_v2_request::AddSubMenu_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ADDSUBMENU_V2_REQUEST),
      position(0)
{
}



bool AddSubMenu_v2_request::set_menuID(unsigned int menuID_)
{
  if(menuID_>2000000000)  return false;
  menuID=menuID_;
  return true;
}

bool AddSubMenu_v2_request::set_position(unsigned int position_)
{
  if(position_>1000)  return false;
  delete position;
  position=0;

  position=new unsigned int(position_);
  return true;
}

void AddSubMenu_v2_request::reset_position(void)
{
  if(position)
    delete position;
  position=0;
}

bool AddSubMenu_v2_request::set_menuName(const std::string& menuName_)
{
  if(menuName_.length()>500)  return false;
  menuName=menuName_;
  return true;
}




unsigned int AddSubMenu_v2_request::get_menuID(void) const
{
  return menuID;
}

const unsigned int* AddSubMenu_v2_request::get_position(void) const 
{
  return position;
}

const std::string& AddSubMenu_v2_request::get_menuName(void) const 
{
  return menuName;
}

