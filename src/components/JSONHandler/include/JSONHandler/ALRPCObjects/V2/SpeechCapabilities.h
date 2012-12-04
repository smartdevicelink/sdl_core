#ifndef NSAPPLINKRPCV2_SPEECHCAPABILITIES_INCLUDE
#define NSAPPLINKRPCV2_SPEECHCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Contains information about the TTS capabilities.

  class SpeechCapabilities
  {
  public:
    enum SpeechCapabilitiesInternal
    {
      INVALID_ENUM=-1,
      SC_TEXT=0,
      SAPI_PHONEMES=1,
      LHPLUS_PHONEMES=2,
      PRE_RECORDED=3,
      SILENCE=4
    };
  
    SpeechCapabilities() : mInternal(INVALID_ENUM)				{}
    SpeechCapabilities(SpeechCapabilitiesInternal e) : mInternal(e)		{}
  
    SpeechCapabilitiesInternal get(void) const	{ return mInternal; }
    void set(SpeechCapabilitiesInternal e)		{ mInternal=e; }
  
  private:
    SpeechCapabilitiesInternal mInternal;
    friend class SpeechCapabilitiesMarshaller;
  };
  
}

#endif
