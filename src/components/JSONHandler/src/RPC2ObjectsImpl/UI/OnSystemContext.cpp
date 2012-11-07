#include "../include/JSONHandler/RPC2Objects/UI/OnSystemContext.h"
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


OnSystemContext& OnSystemContext::operator =(const OnSystemContext& c)
{
  systemContext=c.systemContext;
  return *this;
}


OnSystemContext::~OnSystemContext(void)
{
}


OnSystemContext::OnSystemContext(void) : 
  RPC2Notification(Marshaller::METHOD_ONSYSTEMCONTEXT)
{
}


OnSystemContext::OnSystemContext(const OnSystemContext& c) : RPC2Notification(Marshaller::METHOD_ONSYSTEMCONTEXT)
{
  *this=c;
}


const SystemContext& OnSystemContext::get_systemContext(void)
{
  return systemContext;
}

bool OnSystemContext::set_systemContext(const SystemContext& systemContext_)
{
  systemContext=systemContext_;
  return true;
}

bool OnSystemContext::checkIntegrity(void)
{
  return OnSystemContextMarshaller::checkIntegrity(*this);
}
