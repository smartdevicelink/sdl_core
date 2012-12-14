#ifndef NSAPPLINKRPC_AUDIOSTREAMINGSTATE_INCLUDE
#define NSAPPLINKRPC_AUDIOSTREAMINGSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Enumeraction that describes possible states of audio streaming.

  class AudioStreamingState
  {
  public:
    enum AudioStreamingStateInternal
    {
      INVALID_ENUM=-1,
      AUDIBLE=0,
      NOT_AUDIBLE=1
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
