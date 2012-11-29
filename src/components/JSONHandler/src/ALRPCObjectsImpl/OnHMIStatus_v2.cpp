#include "../include/JSONHandler/ALRPCObjects/OnHMIStatus_v2.h"
#include "OnHMIStatus_v2Marshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "AudioStreamingState_v2Marshaller.h"
#include "HMILevelMarshaller.h"
#include "SystemContext_v2Marshaller.h"

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

OnHMIStatus_v2::~OnHMIStatus_v2(void)
{
}


OnHMIStatus_v2::OnHMIStatus_v2(const OnHMIStatus_v2& c)
{
  *this=c;
}


bool OnHMIStatus_v2::checkIntegrity(void)
{
  return OnHMIStatus_v2Marshaller::checkIntegrity(*this);
}


OnHMIStatus_v2::OnHMIStatus_v2(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONHMISTATUS_V2)
{
}



bool OnHMIStatus_v2::set_hmiLevel(const HMILevel& hmiLevel_)
{
  if(!HMILevelMarshaller::checkIntegrityConst(hmiLevel_))   return false;
  hmiLevel=hmiLevel_;
  return true;
}

bool OnHMIStatus_v2::set_audioStreamingState(const AudioStreamingState_v2& audioStreamingState_)
{
  if(!AudioStreamingState_v2Marshaller::checkIntegrityConst(audioStreamingState_))   return false;
  audioStreamingState=audioStreamingState_;
  return true;
}

bool OnHMIStatus_v2::set_systemContext(const SystemContext_v2& systemContext_)
{
  if(!SystemContext_v2Marshaller::checkIntegrityConst(systemContext_))   return false;
  systemContext=systemContext_;
  return true;
}




const HMILevel& OnHMIStatus_v2::get_hmiLevel(void) const 
{
  return hmiLevel;
}

const AudioStreamingState_v2& OnHMIStatus_v2::get_audioStreamingState(void) const 
{
  return audioStreamingState;
}

const SystemContext_v2& OnHMIStatus_v2::get_systemContext(void) const 
{
  return systemContext;
}

