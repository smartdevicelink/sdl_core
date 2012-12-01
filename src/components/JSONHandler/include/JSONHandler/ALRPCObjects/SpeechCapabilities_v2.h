#ifndef NSAPPLINKRPC_SPEECHCAPABILITIES_V2_INCLUDE
#define NSAPPLINKRPC_SPEECHCAPABILITIES_V2_INCLUDE


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

///  Contains information about the TTS capabilities.

  class SpeechCapabilities_v2
  {
  public:
    enum SpeechCapabilities_v2Internal
    {
      INVALID_ENUM=-1,
      SC_TEXT=0,
      SAPI_PHONEMES=1,
      LHPLUS_PHONEMES=2,
      PRE_RECORDED=3,
      SILENCE=4
    };
  
    SpeechCapabilities_v2() : mInternal(INVALID_ENUM)				{}
    SpeechCapabilities_v2(SpeechCapabilities_v2Internal e) : mInternal(e)		{}
  
    SpeechCapabilities_v2Internal get(void) const	{ return mInternal; }
    void set(SpeechCapabilities_v2Internal e)		{ mInternal=e; }
  
  private:
    SpeechCapabilities_v2Internal mInternal;
    friend class SpeechCapabilities_v2Marshaller;
  };
  
}

#endif
