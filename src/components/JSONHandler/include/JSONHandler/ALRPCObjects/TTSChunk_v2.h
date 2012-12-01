#ifndef NSAPPLINKRPC_TTSCHUNK_V2_INCLUDE
#define NSAPPLINKRPC_TTSCHUNK_V2_INCLUDE

#include <string>

#include "SpeechCapabilities_v2.h"


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

///  A TTS chunk, that consists of the text/phonemes to speak and the type (like text or SAPI)

  class TTSChunk_v2
  {
  public:
  
    TTSChunk_v2(const TTSChunk_v2& c);
    TTSChunk_v2(void);
  
    bool checkIntegrity(void);
  // getters

    const std::string& get_text(void) const;
    const SpeechCapabilities_v2& get_type(void) const;

// setters

    bool set_text(const std::string& text_);
    bool set_type(const SpeechCapabilities_v2& type_);

  private:

    friend class TTSChunk_v2Marshaller;


/**
     The text or phonemes to speak.
     May not be empty.
*/
      std::string text;	//!< (500)

///  Describes, whether it is text or a specific phoneme set. See SpeechCapabilities
      SpeechCapabilities_v2 type;
  };

}

#endif
