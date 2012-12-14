#ifndef NSRPC2COMMUNICATION_NAVIGATION_SHOWCONSTANTTBT_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_SHOWCONSTANTTBT_INCLUDE

#include <string>
#include <vector>
#include "JSONHandler/RPC2Request.h"

#include "../include/JSONHandler/ALRPCObjects/V2/Image.h"
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
      const NsAppLinkRPCV2::Image& get_turnIcon(void);

      float get_distanceToManeuver(void);

      float get_distanceToManeuverScale(void);

      const bool* get_maneuverComplete(void);
      const std::vector< NsAppLinkRPCV2::SoftButton>& get_softButtons(void);

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

      bool set_turnIcon(const NsAppLinkRPCV2::Image& turnIcon);

/// 0 <= distanceToManeuver <= 1000000000
      bool set_distanceToManeuver(float distanceToManeuver);

/// 0 <= distanceToManeuverScale <= 1000000000
      bool set_distanceToManeuverScale(float distanceToManeuverScale);

      bool set_maneuverComplete(const bool& maneuverComplete);

      void reset_maneuverComplete(void);

/// 0 <= size <= 3
      bool set_softButtons(const std::vector< NsAppLinkRPCV2::SoftButton>& softButtons);

      bool set_appId(int appId);


    private:

      friend class ShowConstantTBTMarshaller;

      std::string* navigationText1;
      std::string* navigationText2;
      std::string* eta;
      std::string* totalDistance;
      NsAppLinkRPCV2::Image turnIcon;
      float distanceToManeuver;
      float distanceToManeuverScale;
      bool* maneuverComplete;
      std::vector< NsAppLinkRPCV2::SoftButton> softButtons;
      int appId;

    };
  }
}

#endif
