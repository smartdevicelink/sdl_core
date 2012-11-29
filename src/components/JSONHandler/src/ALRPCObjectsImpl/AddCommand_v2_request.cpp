#include "../include/JSONHandler/ALRPCObjects/AddCommand_v2_request.h"
#include "AddCommand_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ImageMarshaller.h"
#include "MenuParams_v2Marshaller.h"

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
AddCommand_v2_request& AddCommand_v2_request::operator =(const AddCommand_v2_request& c)
{
  cmdID= c.cmdID ? new unsigned int(c.cmdID[0]) : 0;
  menuParams= c.menuParams ? new MenuParams_v2(c.menuParams[0]) : 0;
  vrCommands= c.vrCommands ? new std::vector<std::string>(c.vrCommands[0]) : 0;
  cmdIcon= c.cmdIcon ? new Image(c.cmdIcon[0]) : 0;

  return *this;}


AddCommand_v2_request::~AddCommand_v2_request(void)
{
  if(cmdID)
    delete cmdID;
  if(menuParams)
    delete menuParams;
  if(vrCommands)
    delete vrCommands;
  if(cmdIcon)
    delete cmdIcon;
}


AddCommand_v2_request::AddCommand_v2_request(const AddCommand_v2_request& c)
{
  *this=c;
}


bool AddCommand_v2_request::checkIntegrity(void)
{
  return AddCommand_v2_requestMarshaller::checkIntegrity(*this);
}


AddCommand_v2_request::AddCommand_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ADDCOMMAND_V2_REQUEST),
      cmdID(0),
    menuParams(0),
    vrCommands(0),
    cmdIcon(0)
{
}



bool AddCommand_v2_request::set_cmdID(unsigned int cmdID_)
{
  if(cmdID_>2000000000)  return false;
  delete cmdID;
  cmdID=0;

  cmdID=new unsigned int(cmdID_);
  return true;
}

void AddCommand_v2_request::reset_cmdID(void)
{
  if(cmdID)
    delete cmdID;
  cmdID=0;
}

bool AddCommand_v2_request::set_menuParams(const MenuParams_v2& menuParams_)
{
  if(!MenuParams_v2Marshaller::checkIntegrityConst(menuParams_))   return false;
  delete menuParams;
  menuParams=0;

  menuParams=new MenuParams_v2(menuParams_);
  return true;
}

void AddCommand_v2_request::reset_menuParams(void)
{
  if(menuParams)
    delete menuParams;
  menuParams=0;
}

bool AddCommand_v2_request::set_vrCommands(const std::vector<std::string>& vrCommands_)
{
  unsigned int i=vrCommands_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(vrCommands_[i].length()>99)  return false;
  }
  delete vrCommands;
  vrCommands=0;

  vrCommands=new std::vector<std::string>(vrCommands_);
  return true;
}

void AddCommand_v2_request::reset_vrCommands(void)
{
  if(vrCommands)
    delete vrCommands;
  vrCommands=0;
}

bool AddCommand_v2_request::set_cmdIcon(const Image& cmdIcon_)
{
  if(!ImageMarshaller::checkIntegrityConst(cmdIcon_))   return false;
  delete cmdIcon;
  cmdIcon=0;

  cmdIcon=new Image(cmdIcon_);
  return true;
}

void AddCommand_v2_request::reset_cmdIcon(void)
{
  if(cmdIcon)
    delete cmdIcon;
  cmdIcon=0;
}




const unsigned int* AddCommand_v2_request::get_cmdID(void) const 
{
  return cmdID;
}

const MenuParams_v2* AddCommand_v2_request::get_menuParams(void) const 
{
  return menuParams;
}

const std::vector<std::string>* AddCommand_v2_request::get_vrCommands(void) const 
{
  return vrCommands;
}

const Image* AddCommand_v2_request::get_cmdIcon(void) const 
{
  return cmdIcon;
}

