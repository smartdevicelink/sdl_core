#ifndef APPLINKRPC_SPEECHCAPABILITIES_INCLUDE
#define APPLINKRPC_SPEECHCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

namespace AppLinkRPC
{

///  Contains information about the TTS capabilities.

  class SpeechCapabilities
  {
  public:
    enum SpeechCapabilitiesInternal
    {
      INVALID_ENUM=-1,
      TEXT=0,
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
