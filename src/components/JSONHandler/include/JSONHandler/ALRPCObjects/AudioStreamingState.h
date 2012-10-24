#ifndef AUDIOSTREAMINGSTATE_INCLUDE
#define AUDIOSTREAMINGSTATE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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

#endif
