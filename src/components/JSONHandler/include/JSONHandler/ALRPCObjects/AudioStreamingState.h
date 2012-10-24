#ifndef AUDIOSTREAMINGSTATE_INCLUDE
#define AUDIOSTREAMINGSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

#endif
