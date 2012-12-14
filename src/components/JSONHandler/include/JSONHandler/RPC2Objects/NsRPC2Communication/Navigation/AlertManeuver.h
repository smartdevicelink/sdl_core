#ifndef NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVER_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_ALERTMANEUVER_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V1/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SoftButton.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace Navigation
  {

    class AlertManeuver : public ::NsRPC2Communication::RPC2Request
    {
    public:
    
      AlertManeuver(const AlertManeuver& c);
      AlertManeuver(void);
    
      AlertManeuver& operator =(const AlertManeuver&);
    
      virtual ~AlertManeuver(void);
    
      bool checkIntegrity(void);
    
// getters
      const std::vector< NsAppLinkRPC::TTSChunk>& get_ttsChunks(void);

      const std::vector< NsAppLinkRPCV2::SoftButton>& get_softButtons(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_ttsChunks(const std::vector< NsAppLinkRPC::TTSChunk>& ttsChunks);

/// 0 <= size <= 3
      bool set_softButtons(const std::vector< NsAppLinkRPCV2::SoftButton>& softButtons);

      bool set_appId(int appId);


    private:

      friend class AlertManeuverMarshaller;

      std::vector< NsAppLinkRPC::TTSChunk> ttsChunks;
      std::vector< NsAppLinkRPCV2::SoftButton> softButtons;
      int appId;

    };
  }
}

#endif
