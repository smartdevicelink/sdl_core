#include "../include/JSONHandler/ALRPCObjects/V2/AlertManeuver_request.h"
#include "AlertManeuver_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "SoftButtonMarshaller.h"
#include "TTSChunkMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

AlertManeuver_request::~AlertManeuver_request(void)
{
}


AlertManeuver_request::AlertManeuver_request(const AlertManeuver_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool AlertManeuver_request::checkIntegrity(void)
{
  return AlertManeuver_requestMarshaller::checkIntegrity(*this);
}


AlertManeuver_request::AlertManeuver_request(void) : ALRPC2Message(PROTOCOL_VERSION)
{
}



bool AlertManeuver_request::set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_)
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

bool AlertManeuver_request::set_softButtons(const std::vector<SoftButton>& softButtons_)
{
  unsigned int i=softButtons_.size();
  if(i>3 || i<0)  return false;
  while(i--)
  {
    if(!SoftButtonMarshaller::checkIntegrityConst(softButtons_[i]))   return false;
  }
  softButtons=softButtons_;
  return true;
}




const std::vector<TTSChunk>& AlertManeuver_request::get_ttsChunks(void) const 
{
  return ttsChunks;
}

const std::vector<SoftButton>& AlertManeuver_request::get_softButtons(void) const 
{
  return softButtons;
}

