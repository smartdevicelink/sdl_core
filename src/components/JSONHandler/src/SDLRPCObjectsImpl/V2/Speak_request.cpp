//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/Speak_request.h"
#include "Speak_requestMarshaller.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "TTSChunkMarshaller.h"

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

Speak_request::~Speak_request(void)
{
}


Speak_request::Speak_request(const Speak_request& c) : NsSmartDeviceLinkRPC::SDLRPCMessage(c)
{
  *this=c;
}


bool Speak_request::checkIntegrity(void)
{
  return Speak_requestMarshaller::checkIntegrity(*this);
}


Speak_request::Speak_request(void) : NsSmartDeviceLinkRPC::SDLRPCMessage(PROTOCOL_VERSION)
{
}



bool Speak_request::set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_)
{
  unsigned int i=ttsChunks_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunkMarshaller::checkIntegrityConst(ttsChunks_[i]))   return false;
  }
  ttsChunks=ttsChunks_;
  return true;
}




const std::vector<TTSChunk>& Speak_request::get_ttsChunks(void) const 
{
  return ttsChunks;
}

