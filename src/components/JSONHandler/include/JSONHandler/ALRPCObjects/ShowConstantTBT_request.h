#ifndef SHOWCONSTANTTBT_REQUEST_INCLUDE
#define SHOWCONSTANTTBT_REQUEST_INCLUDE

#include <vector>
#include <string>

#include "Image.h"
#include "SoftButton.h"
#include "../../../JSONHandler/ALRPCRequest.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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
  float distanceToManeuver;	//!<  (0,1000000000)
  float distanceToManeuverScale;	//!<  (0,1000000000)
  bool* maneuverComplete;
  std::vector<SoftButton> softButtons;	//!<   [%s..%s] 
};

#endif
