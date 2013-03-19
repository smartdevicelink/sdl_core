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

#ifndef NSSMARTDEVICELINKKRPCV2_CHOICE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_CHOICE_INCLUDE

#include <string>
#include <vector>

#include "Image.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPCV2
{

///  A choice is an option given to the user, which can be selected either by menu, or through voice recognition system.

  class Choice
  {
  public:
  
    Choice(const Choice& c);
    Choice(void);
  
    bool checkIntegrity(void);
  // getters

    unsigned int get_choiceID(void) const;
    const Image& get_image(void) const;
    const std::string& get_menuName(void) const;
    const std::vector<std::string>& get_vrCommands(void) const;

// setters

    bool set_choiceID(unsigned int choiceID_);
    bool set_image(const Image& image_);
    bool set_menuName(const std::string& menuName_);
    bool set_vrCommands(const std::vector<std::string>& vrCommands_);

  private:

    friend class ChoiceMarshaller;

      unsigned int choiceID;	//!<  (0,65535)
      Image image;
      std::string menuName;	//!< (500)
      std::vector<std::string> vrCommands;	//!<   [%s..%s] (99)
  };

}

#endif
