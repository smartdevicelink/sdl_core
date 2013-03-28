//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef NSSMARTDEVICELINKKRPC_TTSCHUNK_INCLUDE
#define NSSMARTDEVICELINKKRPC_TTSCHUNK_INCLUDE

#include <string>

#include "SpeechCapabilities.h"


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
