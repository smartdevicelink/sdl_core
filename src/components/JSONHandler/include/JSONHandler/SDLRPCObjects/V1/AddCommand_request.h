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

#ifndef NSSMARTDEVICELINKRPC_ADDCOMMAND_REQUEST_INCLUDE
#define NSSMARTDEVICELINKRPC_ADDCOMMAND_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "MenuParams.h"
#include "JSONHandler/SDLRPCRequest.h"


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

/**
     Adds a command to the in application menu.
     Either menuParams or vrCommands must be provided.
*/

  class AddCommand_request : public SDLRPCRequest
  {
  public:
  
    AddCommand_request(const AddCommand_request& c);
    AddCommand_request(void);
    
    virtual ~AddCommand_request(void);
  
    AddCommand_request& operator =(const AddCommand_request&);
  
    bool checkIntegrity(void);

    unsigned int get_cmdID(void) const;
    const MenuParams* get_menuParams(void) const;
    const std::vector<std::string>* get_vrCommands(void) const;

    bool set_cmdID(unsigned int cmdID_);
    void reset_menuParams(void);
    bool set_menuParams(const MenuParams& menuParams_);
    void reset_vrCommands(void);
    bool set_vrCommands(const std::vector<std::string>& vrCommands_);

  private:
  
    friend class AddCommand_requestMarshaller;


///  unique ID of the command to add.
      unsigned int cmdID;	//!<  (0,2000000000)

///  Optional sub value containing menu parameters
      MenuParams* menuParams;

/**
     An array of strings to be used as VR sysnonyms for this command.
     If this array is provided, it may not be empty.
*/
      std::vector<std::string>* vrCommands;	//!<   [%s..%s] (99)
  };

}

#endif
