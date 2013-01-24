#ifndef NSAPPLINKRPC_AUDIOCAPTUREQUALITY_INCLUDE
#define NSAPPLINKRPC_AUDIOCAPTUREQUALITY_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

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
  
}

#endif
