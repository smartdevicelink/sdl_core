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

#ifndef NSRPC2COMMUNICATION_UI_SLIDER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SLIDER_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

namespace NsRPC2Communication
{
  namespace UI
  {

    class Slider : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      Slider(const Slider& c);
      Slider(void);
    
      Slider& operator =(const Slider&);
    
      virtual ~Slider(void);
    
      bool checkIntegrity(void);
    
// getters
      unsigned int get_numTicks(void);

      unsigned int get_position(void);

      const std::string& get_sliderHeader(void);

      const std::vector< std::string>* get_sliderFooter(void);
      unsigned int get_timeout(void);

      int get_appId(void);


// setters
/// 2 <= numTicks <= 26
      bool set_numTicks(unsigned int numTicks);

/// 1 <= position <= 16
      bool set_position(unsigned int position);

/// sliderHeader <= 500
      bool set_sliderHeader(const std::string& sliderHeader);

/// sliderFooter[] <= 500 ; 1 <= size <= 26
      bool set_sliderFooter(const std::vector< std::string>& sliderFooter);

      void reset_sliderFooter(void);

/// timeout <= 65535
      bool set_timeout(unsigned int timeout);

      bool set_appId(int appId);


    private:

      friend class SliderMarshaller;

      unsigned int numTicks;
      unsigned int position;
      std::string sliderHeader;
      std::vector< std::string>* sliderFooter;
      unsigned int timeout;
      int appId;

    };
  }
}

#endif
