#include "../include/JSONHandler/ALRPCObjects/Speak_v2_request.h"
#include "Speak_v2_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "TTSChunk_v2Marshaller.h"

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

Speak_v2_request::~Speak_v2_request(void)
{
}


Speak_v2_request::Speak_v2_request(const Speak_v2_request& c)
{
  *this=c;
}


bool Speak_v2_request::checkIntegrity(void)
{
  return Speak_v2_requestMarshaller::checkIntegrity(*this);
}


Speak_v2_request::Speak_v2_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SPEAK_V2_REQUEST)
{
}



bool Speak_v2_request::set_ttsChunks(const std::vector<TTSChunk_v2>& ttsChunks_)
{
  unsigned int i=ttsChunks_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(ttsChunks_[i]))   return false;
  }
  ttsChunks=ttsChunks_;
  return true;
}




const std::vector<TTSChunk_v2>& Speak_v2_request::get_ttsChunks(void) const 
{
  return ttsChunks;
}

