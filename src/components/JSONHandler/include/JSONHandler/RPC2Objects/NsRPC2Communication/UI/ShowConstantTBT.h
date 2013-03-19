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

#ifndef NSRPC2COMMUNICATION_UI_SHOWCONSTANTTBT_INCLUDE
#define NSRPC2COMMUNICATION_UI_SHOWCONSTANTTBT_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/Image.h"
#include "../include/JSONHandler/SDLRPCObjects/V2/SoftButton.h"

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

    class ShowConstantTBT : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      ShowConstantTBT(const ShowConstantTBT& c);
      ShowConstantTBT(void);
    
      ShowConstantTBT& operator =(const ShowConstantTBT&);
    
      virtual ~ShowConstantTBT(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::string* get_navigationText1(void);
      const std::string* get_navigationText2(void);
      const std::string* get_eta(void);
      const std::string* get_totalDistance(void);
      const NsSmartDeviceLinkRPCV2::Image& get_turnIcon(void);

      float get_distanceToManeuver(void);

      float get_distanceToManeuverScale(void);

      const bool* get_maneuverComplete(void);
      const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& get_softButtons(void);

      int get_appId(void);


// setters
/// navigationText1 <= 500
      bool set_navigationText1(const std::string& navigationText1);

      void reset_navigationText1(void);

/// navigationText2 <= 500
      bool set_navigationText2(const std::string& navigationText2);

      void reset_navigationText2(void);

/// eta <= 500
      bool set_eta(const std::string& eta);

      void reset_eta(void);

/// totalDistance <= 500
      bool set_totalDistance(const std::string& totalDistance);

      void reset_totalDistance(void);

      bool set_turnIcon(const NsSmartDeviceLinkRPCV2::Image& turnIcon);

/// 0 <= distanceToManeuver <= 1000000000
      bool set_distanceToManeuver(float distanceToManeuver);

/// 0 <= distanceToManeuverScale <= 1000000000
      bool set_distanceToManeuverScale(float distanceToManeuverScale);

      bool set_maneuverComplete(const bool& maneuverComplete);

      void reset_maneuverComplete(void);

/// 0 <= size <= 3
      bool set_softButtons(const std::vector< NsSmartDeviceLinkRPCV2::SoftButton>& softButtons);

      bool set_appId(int appId);


    private:

      friend class ShowConstantTBTMarshaller;

      std::string* navigationText1;
      std::string* navigationText2;
      std::string* eta;
      std::string* totalDistance;
      NsSmartDeviceLinkRPCV2::Image turnIcon;
      float distanceToManeuver;
      float distanceToManeuverScale;
      bool* maneuverComplete;
      std::vector< NsSmartDeviceLinkRPCV2::SoftButton> softButtons;
      int appId;

    };
  }
}

#endif
