#ifndef NSAPPLINKRPCV2_AUDIOCAPTUREQUALITY_INCLUDE
#define NSAPPLINKRPCV2_AUDIOCAPTUREQUALITY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Describes different quality options for PerformAudioPassThru.

  class AudioCaptureQuality
  {
  public:
    enum AudioCaptureQualityInternal
    {
      INVALID_ENUM=-1,
      FIX_8_BIT=0,
      FIX_16_BIT=1
    };
  
    AudioCaptureQuality() : mInternal(INVALID_ENUM)				{}
    AudioCaptureQuality(AudioCaptureQualityInternal e) : mInternal(e)		{}
  
    AudioCaptureQualityInternal get(void) const	{ return mInternal; }
    void set(AudioCaptureQualityInternal e)		{ mInternal=e; }
  
  private:
    AudioCaptureQualityInternal mInternal;
    friend class AudioCaptureQualityMarshaller;
  };
  
}

#endif
