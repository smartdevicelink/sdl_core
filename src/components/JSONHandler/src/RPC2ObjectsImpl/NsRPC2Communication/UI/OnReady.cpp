#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnReady.h"
#include "../../../../../Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


OnReady& OnReady::operator =(const OnReady& c)
{
  return *this;
}


OnReady::~OnReady(void)
{
}


OnReady::OnReady(void) : 
  RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONREADY)
{
}


OnReady::OnReady(const OnReady& c) : RPC2Notification(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONREADY)
{
  *this=c;
}


bool OnReady::checkIntegrity(void)
{
  return OnReadyMarshaller::checkIntegrity(*this);
}
