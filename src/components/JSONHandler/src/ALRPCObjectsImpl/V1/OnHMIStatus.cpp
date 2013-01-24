#include "../include/JSONHandler/ALRPCObjects/V1/OnHMIStatus.h"
#include "OnHMIStatusMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "AudioStreamingStateMarshaller.h"
#include "HMILevelMarshaller.h"
#include "SystemContextMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

using namespace NsAppLinkRPC;

OnHMIStatus::~OnHMIStatus(void)
{
}


OnHMIStatus::OnHMIStatus(const OnHMIStatus& c)
{
  *this=c;
}


bool OnHMIStatus::checkIntegrity(void)
{
  return OnHMIStatusMarshaller::checkIntegrity(*this);
}


OnHMIStatus::OnHMIStatus(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONHMISTATUS)
{
}



bool OnHMIStatus::set_hmiLevel(const HMILevel& hmiLevel_)
{
  if(!HMILevelMarshaller::checkIntegrityConst(hmiLevel_))   return false;
  hmiLevel=hmiLevel_;
  return true;
}

bool OnHMIStatus::set_audioStreamingState(const AudioStreamingState& audioStreamingState_)
{
  if(!AudioStreamingStateMarshaller::checkIntegrityConst(audioStreamingState_))   return false;
  audioStreamingState=audioStreamingState_;
  return true;
}

bool OnHMIStatus::set_systemContext(const SystemContext& systemContext_)
{
  if(!SystemContextMarshaller::checkIntegrityConst(systemContext_))   return false;
  systemContext=systemContext_;
  return true;
}




const HMILevel& OnHMIStatus::get_hmiLevel(void) const 
{
  return hmiLevel;
}

const AudioStreamingState& OnHMIStatus::get_audioStreamingState(void) const 
{
  return audioStreamingState;
}

const SystemContext& OnHMIStatus::get_systemContext(void) const 
{
  return systemContext;
}

