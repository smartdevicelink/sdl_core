#ifndef AUDIOTYPE_INCLUDE
#define AUDIOTYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Describes different audio type options for PerformAudioPassThru.

class AudioType
{
public:
  enum AudioTypeInternal
  {
    INVALID_ENUM=-1,
    WAV=0,
    PCM=1,
    MP3=2
  };

  AudioType() : mInternal(INVALID_ENUM)				{}
  AudioType(AudioTypeInternal e) : mInternal(e)		{}

  AudioTypeInternal get(void) const	{ return mInternal; }
  void set(AudioTypeInternal e)		{ mInternal=e; }

private:
  AudioTypeInternal mInternal;
  friend class AudioTypeMarshaller;
};

#endif
