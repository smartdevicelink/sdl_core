#ifndef SPEECHCAPABILITIES_INCLUDE
#define SPEECHCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


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

#endif
