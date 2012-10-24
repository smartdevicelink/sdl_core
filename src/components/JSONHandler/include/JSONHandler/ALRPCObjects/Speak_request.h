#ifndef SPEAK_REQUEST_INCLUDE
#define SPEAK_REQUEST_INCLUDE

#include <vector>

#include "TTSChunk.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Speaks a text.

class Speak_request : public ALRPCRequest
{
public:

  Speak_request(const Speak_request& c);
  Speak_request(void);
  
  virtual ~Speak_request(void);

  bool checkIntegrity(void);

  const std::vector<TTSChunk>& get_ttsChunks(void) const;

  bool set_ttsChunks(const std::vector<TTSChunk>& ttsChunks_);

private:

  friend class Speak_requestMarshaller;

  std::vector<TTSChunk> ttsChunks;	//!<   [%s..%s] 
};

#endif
