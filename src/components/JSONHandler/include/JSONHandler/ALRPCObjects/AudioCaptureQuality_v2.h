#ifndef NSAPPLINKRPC_AUDIOCAPTUREQUALITY_V2_INCLUDE
#define NSAPPLINKRPC_AUDIOCAPTUREQUALITY_V2_INCLUDE


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

///  Describes different quality options for PerformAudioPassThru.

  class AudioCaptureQuality_v2
  {
  public:
    enum AudioCaptureQuality_v2Internal
    {
      INVALID_ENUM=-1,
      FIX_8_BIT=0,
      FIX_16_BIT=1
    };
  
    AudioCaptureQuality_v2() : mInternal(INVALID_ENUM)				{}
    AudioCaptureQuality_v2(AudioCaptureQuality_v2Internal e) : mInternal(e)		{}
  
    AudioCaptureQuality_v2Internal get(void) const	{ return mInternal; }
    void set(AudioCaptureQuality_v2Internal e)		{ mInternal=e; }
  
  private:
    AudioCaptureQuality_v2Internal mInternal;
    friend class AudioCaptureQuality_v2Marshaller;
  };
  
}

#endif
