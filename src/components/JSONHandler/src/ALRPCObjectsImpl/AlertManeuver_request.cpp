#include "../../include/JSONHandler/ALRPCObjects/AlertManeuver_request.h"
#include "AlertManeuver_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "SoftButtonMarshaller.h"
#include "TTSChunkMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


AlertManeuver_request::~AlertManeuver_request(void)
{
}


AlertManeuver_request::AlertManeuver_request(const AlertManeuver_request& c)
{
  *this=c;
}


bool AlertManeuver_request::checkIntegrity(void)
{
  return AlertManeuver_requestMarshaller::checkIntegrity(*this);
}


AlertManeuver_request::AlertManeuver_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ALERTMANEUVER_REQUEST)
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

