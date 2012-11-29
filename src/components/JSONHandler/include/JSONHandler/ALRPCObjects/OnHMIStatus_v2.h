#ifndef NSAPPLINKRPC_ONHMISTATUS_V2_INCLUDE
#define NSAPPLINKRPC_ONHMISTATUS_V2_INCLUDE


#include "AudioStreamingState_v2.h"
#include "HMILevel.h"
#include "SystemContext_v2.h"
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

  class OnHMIStatus_v2 : public ALRPCNotification
  {
  public:
  
    OnHMIStatus_v2(const OnHMIStatus_v2& c);
    OnHMIStatus_v2(void);
    
    virtual ~OnHMIStatus_v2(void);
  
    bool checkIntegrity(void);

    const HMILevel& get_hmiLevel(void) const;
    const AudioStreamingState_v2& get_audioStreamingState(void) const;
    const SystemContext_v2& get_systemContext(void) const;

    bool set_hmiLevel(const HMILevel& hmiLevel_);
    bool set_audioStreamingState(const AudioStreamingState_v2& audioStreamingState_);
    bool set_systemContext(const SystemContext_v2& systemContext_);

  private:
  
    friend class OnHMIStatus_v2Marshaller;


///  See HMILevel
      HMILevel hmiLevel;

///  See AudioStreamingState
      AudioStreamingState_v2 audioStreamingState;

///  See SystemContext
      SystemContext_v2 systemContext;
  };

}

#endif
