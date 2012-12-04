#ifndef NSAPPLINKRPCV2_ONHMISTATUS_INCLUDE
#define NSAPPLINKRPCV2_ONHMISTATUS_INCLUDE


#include "AudioStreamingState.h"
#include "HMILevel.h"
#include "SystemContext.h"
#include "JSONHandler/ALRPCMessage.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  class OnHMIStatus : public NsAppLinkRPC::ALRPCMessage
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
