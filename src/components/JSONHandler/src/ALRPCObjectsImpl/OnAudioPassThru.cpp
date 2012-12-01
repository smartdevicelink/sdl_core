#include "../include/JSONHandler/ALRPCObjects/OnAudioPassThru.h"
#include "OnAudioPassThruMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

OnAudioPassThru::~OnAudioPassThru(void)
{
}


OnAudioPassThru::OnAudioPassThru(const OnAudioPassThru& c)
{
  *this=c;
}


bool OnAudioPassThru::checkIntegrity(void)
{
  return OnAudioPassThruMarshaller::checkIntegrity(*this);
}


OnAudioPassThru::OnAudioPassThru(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONAUDIOPASSTHRU)
{
}






