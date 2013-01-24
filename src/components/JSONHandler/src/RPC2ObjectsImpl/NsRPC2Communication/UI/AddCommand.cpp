#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


AddCommand& AddCommand::operator =(const AddCommand& c)
{
  cmdId=c.cmdId;
  menuParams=c.menuParams;
  if(cmdIcon)  delete cmdIcon;
  cmdIcon= c.cmdIcon ? new NsAppLinkRPCV2::Image(c.cmdIcon[0]) : 0;
  appId=c.appId;
  return *this;
}


AddCommand::~AddCommand(void)
{
  if(cmdIcon)  delete cmdIcon;
}


AddCommand::AddCommand(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMAND),
  cmdIcon(0)
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

const NsAppLinkRPCV2::MenuParams& AddCommand::get_menuParams(void)
{
  return menuParams;
}

bool AddCommand::set_menuParams(const NsAppLinkRPCV2::MenuParams& menuParams_)
{
  menuParams=menuParams_;
  return true;
}

const NsAppLinkRPCV2::Image* AddCommand::get_cmdIcon(void)
{
  return cmdIcon;
}

bool AddCommand::set_cmdIcon(const NsAppLinkRPCV2::Image& cmdIcon_)
{
  if(cmdIcon)  delete cmdIcon;
  cmdIcon=new NsAppLinkRPCV2::Image(cmdIcon_);
  return true;
}

void AddCommand::reset_cmdIcon(void)
{
  if(cmdIcon)  delete cmdIcon;
  cmdIcon=0;
}

int AddCommand::get_appId(void)
{
  return appId;
}

bool AddCommand::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool AddCommand::checkIntegrity(void)
{
  return AddCommandMarshaller::checkIntegrity(*this);
}
