#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "../src/../include/JSONHandler/RPC2Objects//Marshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;


OnAppRegistered& OnAppRegistered::operator =(const OnAppRegistered& c)
{
  application=c.application;
  return *this;
}


OnAppRegistered::~OnAppRegistered(void)
{
}


OnAppRegistered::OnAppRegistered(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED)
{
}


OnAppRegistered::OnAppRegistered(const OnAppRegistered& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPREGISTERED)
{
  *this=c;
}


const NsAppLinkRPCV2::HMIApplication& OnAppRegistered::get_application(void)
{
  return application;
}

bool OnAppRegistered::set_application(const NsAppLinkRPCV2::HMIApplication& application_)
{
  application=application_;
  return true;
}

bool OnAppRegistered::checkIntegrity(void)
{
  return OnAppRegisteredMarshaller::checkIntegrity(*this);
}
