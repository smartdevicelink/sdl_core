#include "../include/JSONHandler/ALRPCObjects/V2/OnHMIStatus.h"
#include "OnHMIStatusMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "AudioStreamingStateMarshaller.h"
#include "HMILevelMarshaller.h"
#include "SystemContextMarshaller.h"

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

OnHMIStatus::~OnHMIStatus(void)
{
}


OnHMIStatus::OnHMIStatus(const OnHMIStatus& c) : ALRPC2Message(c)
{
  *this=c;
}


bool OnHMIStatus::checkIntegrity(void)
{
  return OnHMIStatusMarshaller::checkIntegrity(*this);
}


OnHMIStatus::OnHMIStatus(void) : ALRPC2Message(PROTOCOL_VERSION)
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

