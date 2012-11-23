#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


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
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND)
{
}


AddCommand::AddCommand(const AddCommand& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND,c.getId())
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

const NsAppLinkRPC::MenuParams& AddCommand::get_menuParams(void)
{
  return menuParams;
}

bool AddCommand::set_menuParams(const NsAppLinkRPC::MenuParams& menuParams_)
{
  menuParams=menuParams_;
  return true;
}

bool AddCommand::checkIntegrity(void)
{
  return AddCommandMarshaller::checkIntegrity(*this);
}
