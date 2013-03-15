//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnSystemContext.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::UI;


OnSystemContext& OnSystemContext::operator =(const OnSystemContext& c)
{
  systemContext=c.systemContext;
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


const NsSmartDeviceLinkRPCV2::SystemContext& OnSystemContext::get_systemContext(void)
{
  return systemContext;
}

bool OnSystemContext::set_systemContext(const NsSmartDeviceLinkRPCV2::SystemContext& systemContext_)
{
  systemContext=systemContext_;
  return true;
}

bool OnSystemContext::checkIntegrity(void)
{
  return OnSystemContextMarshaller::checkIntegrity(*this);
}
