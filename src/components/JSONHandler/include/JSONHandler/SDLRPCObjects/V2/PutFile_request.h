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

#ifndef NSSMARTDEVICELINKKRPCV2_PUTFILE_REQUEST_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_PUTFILE_REQUEST_INCLUDE

#include <string>

#include "FileType.h"
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

/**
     Used to push a binary data onto the SYNC module from a mobile device, such as icons and album art
     Not supported on first generation SYNC vehicles. 
*/

  class PutFile_request : public NsSmartDeviceLinkRPC::SDLRPCMessage
  {
  public:
  
    PutFile_request(const PutFile_request& c);
    PutFile_request(void);
    
    virtual ~PutFile_request(void);
  
    PutFile_request& operator =(const PutFile_request&);
  
    bool checkIntegrity(void);

    const std::string& get_syncFileName(void) const;
    const FileType& get_fileType(void) const;
    const bool* get_persistentFile(void) const;
    const std::string* get_fileData(void) const;

    bool set_syncFileName(const std::string& syncFileName_);
    bool set_fileType(const FileType& fileType_);
    void reset_persistentFile(void);
    bool set_persistentFile(bool persistentFile_);
    void reset_fileData(void);
    bool set_fileData(const std::string& fileData_);

  private:
  
    friend class PutFile_requestMarshaller;


///  File reference name.
      std::string syncFileName;	//!< (500)

///  Selected file type.
      FileType fileType;

/**
     Indicates if the file is meant to persist between sessions / ignition cycles.
     If set to TRUE, then the system will aim to persist this file through session / cycles.
     While files with this designation will have priority over others, they are subject to deletion by the system at any time.
     In the event of automatic deletion by the system, the app will receive a rejection and have to resend the file.
     If omitted, the value will be set to false.
*/
      bool* persistentFile;

///  Binary data.
      std::string* fileData;
  };

}

#endif
