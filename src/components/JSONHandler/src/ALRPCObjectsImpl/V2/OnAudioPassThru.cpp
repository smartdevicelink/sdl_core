#include "../include/JSONHandler/ALRPCObjects/V2/OnAudioPassThru.h"
#include "OnAudioPassThruMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnAudioPassThru::~OnAudioPassThru(void)
{
}


OnAudioPassThru::OnAudioPassThru(const OnAudioPassThru& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool OnAudioPassThru::checkIntegrity(void)
{
  return OnAudioPassThruMarshaller::checkIntegrity(*this);
}


OnAudioPassThru::OnAudioPassThru(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}






