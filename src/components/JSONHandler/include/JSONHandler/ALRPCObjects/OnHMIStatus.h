#ifndef ONHMISTATUS_INCLUDE
#define ONHMISTATUS_INCLUDE


#include "AudioStreamingState.h"
#include "HMILevel.h"
#include "SystemContext.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


class OnHMIStatus : public ALRPCNotification
{
public:

  OnHMIStatus(const OnHMIStatus& c);
  OnHMIStatus(void);
  
  virtual ~OnHMIStatus(void);

  bool checkIntegrity(void);

  const HMILevel& get_hmiLevel(void) const;
  const AudioStreamingState& get_audioStreamingState(void) const;
  const SystemContext& get_systemContext(void) const;

  bool set_hmiLevel(const HMILevel& hmiLevel_);
  bool set_audioStreamingState(const AudioStreamingState& audioStreamingState_);
  bool set_systemContext(const SystemContext& systemContext_);

private:

  friend class OnHMIStatusMarshaller;

  HMILevel hmiLevel;
  AudioStreamingState audioStreamingState;
  SystemContext systemContext;
};

#endif
