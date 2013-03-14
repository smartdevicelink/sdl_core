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
  author	robok0der
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
