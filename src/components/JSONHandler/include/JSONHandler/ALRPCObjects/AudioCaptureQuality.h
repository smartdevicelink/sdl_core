#ifndef AUDIOCAPTUREQUALITY_INCLUDE
#define AUDIOCAPTUREQUALITY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


///  Describes different quality options for PerformAudioCapture.

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
