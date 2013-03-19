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

#ifndef NSSMARTDEVICELINKKRPC_MENUPARAMS_INCLUDE
#define NSSMARTDEVICELINKKRPC_MENUPARAMS_INCLUDE

#include <string>



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

  class MenuParams
  {
  public:
  
    MenuParams(const MenuParams& c);
    MenuParams(void);
  
    bool checkIntegrity(void);
  
    ~MenuParams(void);
    MenuParams& operator =(const MenuParams&);

// getters

    const std::string& get_menuName(void) const;
    const unsigned int* get_parentID(void) const;
    const unsigned int* get_position(void) const;

// setters

    bool set_menuName(const std::string& menuName_);
    void reset_parentID(void);
    bool set_parentID(unsigned int parentID_);
    void reset_position(void);
    bool set_position(unsigned int position_);

  private:

    friend class MenuParamsMarshaller;


///  Text to show in the menu for this sub menu.
      std::string menuName;	//!< (500)

/**
     unique ID of the sub menu, the command will be added to.
     If not provided, it will be provided to the top level of the in application menu.
*/
      unsigned int* parentID;	//!<  (0,2000000000)

/**
     Position within the items that are are at top level of the in application menu.
     0 will insert at the front.
     1 will insert at the second position.
     if position is greater or equal than the number of items on top level, the sub menu will be appended to the end.
     If this param was omitted the entry will be added at the end.
*/
      unsigned int* position;	//!<  (0,1000)
  };

}

#endif
