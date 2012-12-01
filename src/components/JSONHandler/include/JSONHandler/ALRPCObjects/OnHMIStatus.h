#ifndef NSAPPLINKRPC_ONHMISTATUS_INCLUDE
#define NSAPPLINKRPC_ONHMISTATUS_INCLUDE


#include "AudioStreamingState.h"
#include "HMILevel.h"
#include "SystemContext.h"
#include "JSONHandler/ALRPCNotification.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

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


///  See HMILevel
      HMILevel hmiLevel;

///  See AudioStreamingState
      AudioStreamingState audioStreamingState;

///  See SystemContext
      SystemContext systemContext;
  };

}

#endif
