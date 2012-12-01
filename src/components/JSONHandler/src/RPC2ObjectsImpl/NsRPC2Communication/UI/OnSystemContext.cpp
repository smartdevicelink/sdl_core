#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnSystemContext.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnSystemContext& OnSystemContext::operator =(const OnSystemContext& c)
{
  systemContext=c.systemContext;
  appId=c.appId;
  return *this;
}


OnSystemContext::~OnSystemContext(void)
{
}


OnSystemContext::OnSystemContext(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT)
{
}


OnSystemContext::OnSystemContext(const OnSystemContext& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT)
{
  *this=c;
}


const NsAppLinkRPC::SystemContext& OnSystemContext::get_systemContext(void)
{
  return systemContext;
}

bool OnSystemContext::set_systemContext(const NsAppLinkRPC::SystemContext& systemContext_)
{
  systemContext=systemContext_;
  return true;
}

int OnSystemContext::get_appId(void)
{
  return appId;
}

bool OnSystemContext::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool OnSystemContext::checkIntegrity(void)
{
  return OnSystemContextMarshaller::checkIntegrity(*this);
}
