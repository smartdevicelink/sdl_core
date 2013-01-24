#ifndef NSAPPLINKRPCV2_TTSCHUNK_INCLUDE
#define NSAPPLINKRPCV2_TTSCHUNK_INCLUDE

#include <string>

#include "SpeechCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

///  A TTS chunk, that consists of the text/phonemes to speak and the type (like text or SAPI)

  class TTSChunk
  {
  public:
  
    TTSChunk(const TTSChunk& c);
    TTSChunk(void);
  
    bool checkIntegrity(void);
  // getters

    const std::string& get_text(void) const;
    const SpeechCapabilities& get_type(void) const;

// setters

    bool set_text(const std::string& text_);
    bool set_type(const SpeechCapabilities& type_);

  private:

    friend class TTSChunkMarshaller;


/**
     The text or phonemes to speak.
     May not be empty.
*/
      std::string text;	//!< (500)

///  Describes, whether it is text or a specific phoneme set. See SpeechCapabilities
      SpeechCapabilities type;
  };

}

#endif
