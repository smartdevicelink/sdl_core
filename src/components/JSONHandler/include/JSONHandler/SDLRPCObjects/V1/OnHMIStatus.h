#ifndef NSAPPLINKRPC_ONHMISTATUS_INCLUDE
#define NSAPPLINKRPC_ONHMISTATUS_INCLUDE


#include "AudioStreamingState.h"
#include "HMILevel.h"
#include "SystemContext.h"
#include "JSONHandler/SDLRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
{

  class OnHMIStatus : public SDLRPCNotification
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


///  See HMILevel
      HMILevel hmiLevel;

///  See AudioStreamingState
      AudioStreamingState audioStreamingState;

///  See SystemContext
      SystemContext systemContext;
  };

}

#endif
