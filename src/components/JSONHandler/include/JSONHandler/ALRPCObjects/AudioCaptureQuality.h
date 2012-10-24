#ifndef AUDIOCAPTUREQUALITY_INCLUDE
#define AUDIOCAPTUREQUALITY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Describes different quality options for PerformAudioPassThru.

class AudioCaptureQuality
{
public:
  enum AudioCaptureQualityInternal
  {
    INVALID_ENUM=-1,
    FIX_8KHZ=0,
    FIX_16KHZ=1
  };

  AudioCaptureQuality() : mInternal(INVALID_ENUM)				{}
  AudioCaptureQuality(AudioCaptureQualityInternal e) : mInternal(e)		{}

  AudioCaptureQualityInternal get(void) const	{ return mInternal; }
  void set(AudioCaptureQualityInternal e)		{ mInternal=e; }

private:
  AudioCaptureQualityInternal mInternal;
  friend class AudioCaptureQualityMarshaller;
};

#endif
