//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_SPEECHCAPABILITIES_INCLUDE
#define NSSMARTDEVICELINKKRPC_SPEECHCAPABILITIES_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
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
