#ifndef ONHMISTATUS_CLASS
#define ONHMISTATUS_CLASS value

#include "JSONHandler/MobileRPCNotification.h"
#include "JSONHandler/HMILevel.h"
#include "JSONHandler/AudioStreamingState.h"
#include "JSONHandler/SystemContext.h"

#include <string>

class OnHMIStatus : public MobileRPCNotification
{
public:
  OnHMIStatus( unsigned int protocolVersion ): MobileRPCNotification( protocolVersion, "OnHMIStatus" ){}
  ~OnHMIStatus() {}

  void setHMILevel( HMILevel _HMILevel ) { mHMILevel = _HMILevel; }
  bool getHMILevel() const { return mHMILevel; }

  void setAudioStreamingState( AudioStreamingState _AudioStreamingState ) { mAudioStreamingState = _AudioStreamingState; }
  AudioStreamingState getAudioStreamingState() const { return mAudioStreamingState; }

  void setSystemContext( SystemContext _SystemContext ) { mSystemContext = _SystemContext; }
  SystemContext getSystemContext() const { return mSystemContext; }

  /* data */
private:
  HMILevel 		mHMILevel;
  AudioStreamingState   mAudioStreamingState;
  SystemContext 	mSystemContext;
};

#endif  // ONHMISTATUS_CLASS
