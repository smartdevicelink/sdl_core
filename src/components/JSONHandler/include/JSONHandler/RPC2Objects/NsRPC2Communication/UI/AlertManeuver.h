#ifndef NSRPC2COMMUNICATION_UI_ALERTMANEUVER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ALERTMANEUVER_INCLUDE

#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V2/TTSChunk.h"
#include "../include/JSONHandler/ALRPCObjects/V2/SoftButton.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

namespace NsRPC2Communication
{
  namespace UI
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
      const std::vector< NsAppLinkRPCV2::TTSChunk>& get_ttsChunks(void);

      const std::vector< NsAppLinkRPCV2::SoftButton>& get_softButtons(void);

      int get_appId(void);


// setters
/// 1 <= size <= 100
      bool set_ttsChunks(const std::vector< NsAppLinkRPCV2::TTSChunk>& ttsChunks);

/// 0 <= size <= 3
      bool set_softButtons(const std::vector< NsAppLinkRPCV2::SoftButton>& softButtons);

      bool set_appId(int appId);


    private:

      friend class AlertManeuverMarshaller;

      std::vector< NsAppLinkRPCV2::TTSChunk> ttsChunks;
      std::vector< NsAppLinkRPCV2::SoftButton> softButtons;
      int appId;

    };
  }
}

#endif
