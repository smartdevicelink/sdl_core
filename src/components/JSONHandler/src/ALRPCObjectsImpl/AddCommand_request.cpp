#include "../../include/JSONHandler/ALRPCObjects/AddCommand_request.h"
#include "AddCommand_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "MenuParamsMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

AddCommand_request& AddCommand_request::operator =(const AddCommand_request& c)
{
  cmdID= c.cmdID;
  menuParams= c.menuParams ? new MenuParams(c.menuParams[0]) : 0;
  vrCommands= c.vrCommands ? new std::vector<std::string>(c.vrCommands[0]) : 0;

  return *this;}


AddCommand_request::~AddCommand_request(void)
{
  if(menuParams)
    delete menuParams;
  if(vrCommands)
    delete vrCommands;
}


AddCommand_request::AddCommand_request(const AddCommand_request& c)
{
  *this=c;
}


bool AddCommand_request::checkIntegrity(void)
{
  return AddCommand_requestMarshaller::checkIntegrity(*this);
}


AddCommand_request::AddCommand_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ADDCOMMAND_REQUEST),
      menuParams(0),
    vrCommands(0)
{
}



bool AddCommand_request::set_cmdID(unsigned int cmdID_)
{
  if(cmdID_>2000000000)  return false;
  cmdID=cmdID_;
  return true;
}

bool AddCommand_request::set_menuParams(const MenuParams& menuParams_)
{
  if(!MenuParamsMarshaller::checkIntegrityConst(menuParams_))   return false;
  delete menuParams;
  menuParams=0;

  menuParams=new MenuParams(menuParams_);
  return true;
}

void AddCommand_request::reset_menuParams(void)
{
  if(menuParams)
    delete menuParams;
  menuParams=0;
}

bool AddCommand_request::set_vrCommands(const std::vector<std::string>& vrCommands_)
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

void AddCommand_request::reset_vrCommands(void)
{
  if(vrCommands)
    delete vrCommands;
  vrCommands=0;
}




unsigned int AddCommand_request::get_cmdID(void) const
{
  return cmdID;
}

const MenuParams* AddCommand_request::get_menuParams(void) const 
{
  return menuParams;
}

const std::vector<std::string>* AddCommand_request::get_vrCommands(void) const 
{
  return vrCommands;
}

