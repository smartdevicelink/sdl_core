#ifndef NSAPPLINKRPC_AUDIOSTREAMINGSTATE_V2_INCLUDE
#define NSAPPLINKRPC_AUDIOSTREAMINGSTATE_V2_INCLUDE


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

///  Enumeraction that describes possible states of audio streaming.

  class AudioStreamingState_v2
  {
  public:
    enum AudioStreamingState_v2Internal
    {
      INVALID_ENUM=-1,
      AUDIBLE=0,
      ATTENUATED=1,
      NOT_AUDIBLE=2
    };
  
    AudioStreamingState_v2() : mInternal(INVALID_ENUM)				{}
    AudioStreamingState_v2(AudioStreamingState_v2Internal e) : mInternal(e)		{}
  
    AudioStreamingState_v2Internal get(void) const	{ return mInternal; }
    void set(AudioStreamingState_v2Internal e)		{ mInternal=e; }
  
  private:
    AudioStreamingState_v2Internal mInternal;
    friend class AudioStreamingState_v2Marshaller;
  };
  
}

#endif
