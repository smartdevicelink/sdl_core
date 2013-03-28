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

#ifndef NSSMARTDEVICELINKKRPCV2_TIRESTATUS_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_TIRESTATUS_INCLUDE


#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"
#include "TirePressureTellTale.h"
#include "SingleTireStatus.h"
#include "SingleTireStatus.h"


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

///  The status and pressure of the tires.

  class TireStatus
  {
  public:
  
    TireStatus(const TireStatus& c);
    TireStatus(void);
  
    bool checkIntegrity(void);
  // getters

    const SingleTireStatus& get_innerLeftRear(void) const;
    const SingleTireStatus& get_innerRightRear(void) const;
    const SingleTireStatus& get_leftFront(void) const;
    const SingleTireStatus& get_leftRear(void) const;
    const TirePressureTellTale& get_pressureTellTale(void) const;
    const SingleTireStatus& get_rightFront(void) const;
    const SingleTireStatus& get_rightRear(void) const;

// setters

    bool set_innerLeftRear(const SingleTireStatus& innerLeftRear_);
    bool set_innerRightRear(const SingleTireStatus& innerRightRear_);
    bool set_leftFront(const SingleTireStatus& leftFront_);
    bool set_leftRear(const SingleTireStatus& leftRear_);
    bool set_pressureTellTale(const TirePressureTellTale& pressureTellTale_);
    bool set_rightFront(const SingleTireStatus& rightFront_);
    bool set_rightRear(const SingleTireStatus& rightRear_);

  private:

    friend class TireStatusMarshaller;


///  The status of the inner left rear.
      SingleTireStatus innerLeftRear;

///  The status of the inner right rear.
      SingleTireStatus innerRightRear;

///  The status of the left front tire.
      SingleTireStatus leftFront;

///  The status of the left rear tire.
      SingleTireStatus leftRear;

///  See TirePressureTellTale.
      TirePressureTellTale pressureTellTale;

///  The status of the right front tire.
      SingleTireStatus rightFront;

///  The status of the right rear tire.
      SingleTireStatus rightRear;
  };

}

#endif
