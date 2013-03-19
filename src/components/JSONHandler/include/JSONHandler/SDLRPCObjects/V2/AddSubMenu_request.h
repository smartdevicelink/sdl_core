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

#ifndef NSSMARTDEVICELINKKRPCV2_ADDSUBMENU_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_ADDSUBMENU_REQUEST_INCLUDE

#include <string>

#include "JSONHandler/SDLRPCMessage.h"


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

///  Adds a sub menu to the in-application menu.

  class AddSubMenu_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    AddSubMenu_request(const AddSubMenu_request& c);
    AddSubMenu_request(void);
    
    virtual ~AddSubMenu_request(void);
  
    AddSubMenu_request& operator =(const AddSubMenu_request&);
  
    bool checkIntegrity(void);

    unsigned int get_menuID(void) const;
    const unsigned int* get_position(void) const;
    const std::string& get_menuName(void) const;

    bool set_menuID(unsigned int menuID_);
    void reset_position(void);
    bool set_position(unsigned int position_);
    bool set_menuName(const std::string& menuName_);

  private:
  
    friend class AddSubMenu_requestMarshaller;


///  unique ID of the sub menu to add.
      unsigned int menuID;	//!<  (0,2000000000)

/**
     Position within the items that are are at top level of the in application menu.
     0 will insert at the front.
     1 will insert at the second position.
     If position is greater or equal than the number of items on top level, the sub menu will be appended to the end.
     Position of any submenu will always be located before the return and exit options
     If this param was omitted the entry will be added at the end.
*/
      unsigned int* position;	//!<  (0,1000)

///  Text to show in the menu for this sub menu.
      std::string menuName;	//!< (500)
  };

}

#endif
