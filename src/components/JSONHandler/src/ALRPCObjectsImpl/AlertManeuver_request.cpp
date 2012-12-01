#include "../include/JSONHandler/ALRPCObjects/AlertManeuver_request.h"
#include "AlertManeuver_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "SoftButtonMarshaller.h"
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



bool AlertManeuver_request::set_ttsChunks(const std::vector<TTSChunk_v2>& ttsChunks_)
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




const std::vector<TTSChunk_v2>& AlertManeuver_request::get_ttsChunks(void) const 
{
  return ttsChunks;
}

const std::vector<SoftButton>& AlertManeuver_request::get_softButtons(void) const 
{
  return softButtons;
}

