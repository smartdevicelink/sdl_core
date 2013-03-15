//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/OnAudioPassThru.h"
#include "OnAudioPassThruMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;

OnAudioPassThru::~OnAudioPassThru(void)
{
}


OnAudioPassThru::OnAudioPassThru(const OnAudioPassThru& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool OnAudioPassThru::checkIntegrity(void)
{
  return OnAudioPassThruMarshaller::checkIntegrity(*this);
}


OnAudioPassThru::OnAudioPassThru(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}






