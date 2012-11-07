#include "../include/JSONHandler/RPC2Objects/UI/AddCommand.h"
#include "../include/JSONHandler/RPC2Objects/UI/Marshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;


AddCommand& AddCommand::operator =(const AddCommand& c)
{
  cmdId=c.cmdId;
  menuParams=c.menuParams;
  return *this;
}


AddCommand::~AddCommand(void)
{
}


AddCommand::AddCommand(void) : 
  RPC2Request(Marshaller::METHOD_ADDCOMMAND)
{
}


AddCommand::AddCommand(const AddCommand& c) : RPC2Request(Marshaller::METHOD_ADDCOMMAND,c.getId())
{
  *this=c;
}


unsigned int AddCommand::get_cmdId(void)
{
  return cmdId;
}

bool AddCommand::set_cmdId(unsigned int cmdId_)
{
  cmdId=cmdId_;
  return true;
}

const MenuParams& AddCommand::get_menuParams(void)
{
  return menuParams;
}

bool AddCommand::set_menuParams(const MenuParams& menuParams_)
{
  menuParams=menuParams_;
  return true;
}

bool AddCommand::checkIntegrity(void)
{
  return AddCommandMarshaller::checkIntegrity(*this);
}
