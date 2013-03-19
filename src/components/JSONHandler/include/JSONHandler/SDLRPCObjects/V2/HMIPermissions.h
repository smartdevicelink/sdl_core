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

#ifndef NSSMARTDEVICELINKKRPCV2_HMIPERMISSIONS_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_HMIPERMISSIONS_INCLUDE

#include <vector>

#include "HMILevel.h"
#include "HMILevel.h"


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

  class HMIPermissions
  {
  public:
  
    HMIPermissions(const HMIPermissions& c);
    HMIPermissions(void);
  
    bool checkIntegrity(void);
  // getters

    const std::vector<HMILevel>& get_allowed(void) const;
    const std::vector<HMILevel>& get_userDisallowed(void) const;

// setters

    bool set_allowed(const std::vector<HMILevel>& allowed_);
    bool set_userDisallowed(const std::vector<HMILevel>& userDisallowed_);

  private:

    friend class HMIPermissionsMarshaller;


///  A set of all HMI levels that are permitted for this given RPC.
      std::vector<HMILevel> allowed;	//!<   [%s..%s] 

///  A set of all HMI levels that are prohibited for this given RPC.
      std::vector<HMILevel> userDisallowed;	//!<   [%s..%s] 
  };

}

#endif
