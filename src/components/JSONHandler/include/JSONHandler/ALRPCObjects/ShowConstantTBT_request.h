#ifndef NSAPPLINKRPC_SHOWCONSTANTTBT_REQUEST_INCLUDE
#define NSAPPLINKRPC_SHOWCONSTANTTBT_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Image.h"
#include "SoftButton.h"
#include "JSONHandler/ALRPCRequest.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  class ShowConstantTBT_request : public ALRPCRequest
  {
  public:
  
    ShowConstantTBT_request(const ShowConstantTBT_request& c);
    ShowConstantTBT_request(void);
    
    virtual ~ShowConstantTBT_request(void);
  
    ShowConstantTBT_request& operator =(const ShowConstantTBT_request&);
  
    bool checkIntegrity(void);

    const std::string* get_navigationText1(void) const;
    const std::string* get_navigationText2(void) const;
    const std::string* get_eta(void) const;
    const std::string* get_totalDistance(void) const;
    const Image& get_turnIcon(void) const;
    float get_distanceToManeuver(void) const;
    float get_distanceToManeuverScale(void) const;
    const bool* get_maneuverComplete(void) const;
    const std::vector<SoftButton>& get_softButtons(void) const;

    void reset_navigationText1(void);
    bool set_navigationText1(const std::string& navigationText1_);
    void reset_navigationText2(void);
    bool set_navigationText2(const std::string& navigationText2_);
    void reset_eta(void);
    bool set_eta(const std::string& eta_);
    void reset_totalDistance(void);
    bool set_totalDistance(const std::string& totalDistance_);
    bool set_turnIcon(const Image& turnIcon_);
    bool set_distanceToManeuver(float distanceToManeuver_);
    bool set_distanceToManeuverScale(float distanceToManeuverScale_);
    void reset_maneuverComplete(void);
    bool set_maneuverComplete(bool maneuverComplete_);
    bool set_softButtons(const std::vector<SoftButton>& softButtons_);

  private:
  
    friend class ShowConstantTBT_requestMarshaller;

      std::string* navigationText1;	//!< (500)
      std::string* navigationText2;	//!< (500)
      std::string* eta;	//!< (500)
      std::string* totalDistance;	//!< (500)
      Image turnIcon;

/**
     Distance till next maneuver (starting from) from previous maneuver.
     Used to calculate progress bar.
*/
      float distanceToManeuver;	//!<  (0,1000000000)

/**
     Fraction of distance till next maneuver (starting from when AlertManeuver is triggered).
     Used to calculate progress bar.
*/
      float distanceToManeuverScale;	//!<  (0,1000000000)

/**
     If and when a maneuver has completed while an AlertManeuver is active, the app must send this value set to TRUE in order to clear the AlertManeuver overlay.
     If omitted the value will be assumed as FALSE.
*/
      bool* maneuverComplete;

/**
     Three dynamic SoftButtons available (second SoftButton is fixed to "Turns").
     If omitted on supported displays, the currently displayed SoftButton values will not change.
*/
      std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
  };

}

#endif
