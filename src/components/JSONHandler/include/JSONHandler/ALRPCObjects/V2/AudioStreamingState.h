#ifndef NSAPPLINKRPCV2_AUDIOSTREAMINGSTATE_INCLUDE
#define NSAPPLINKRPCV2_AUDIOSTREAMINGSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Enumeraction that describes possible states of audio streaming.

  class AudioStreamingState
  {
  public:
    enum AudioStreamingStateInternal
    {
      INVALID_ENUM=-1,
      AUDIBLE=0,
      ATTENUATED=1,
      NOT_AUDIBLE=2
    };
  
    AudioStreamingState() : mInternal(INVALID_ENUM)				{}
    AudioStreamingState(AudioStreamingStateInternal e) : mInternal(e)		{}
  
    AudioStreamingStateInternal get(void) const	{ return mInternal; }
    void set(AudioStreamingStateInternal e)		{ mInternal=e; }
  
  private:
    AudioStreamingStateInternal mInternal;
    friend class AudioStreamingStateMarshaller;
  };
  
}

#endif
