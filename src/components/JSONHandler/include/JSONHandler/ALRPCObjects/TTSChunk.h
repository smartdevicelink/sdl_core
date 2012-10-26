#ifndef TTSCHUNK_INCLUDE
#define TTSCHUNK_INCLUDE

#include <string>

#include "SpeechCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



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

  std::string text;	//!< (500)
  SpeechCapabilities type;
};

#endif
