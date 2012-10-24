#ifndef SPEECHCAPABILITIES_INCLUDE
#define SPEECHCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
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
    PRE_RECORDED=2,
    SILENCE=3
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
