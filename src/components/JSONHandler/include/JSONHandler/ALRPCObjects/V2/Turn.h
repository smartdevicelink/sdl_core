#ifndef NSAPPLINKRPCV2_TURN_INCLUDE
#define NSAPPLINKRPCV2_TURN_INCLUDE

#include <string>

#include "Image.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

  class Turn
  {
  public:
  
    Turn(const Turn& c);
    Turn(void);
  
    bool checkIntegrity(void);
  
    ~Turn(void);
    Turn& operator =(const Turn&);

// getters

    const std::string* get_navigationText(void) const;
    const Image& get_turnIcon(void) const;

// setters

    void reset_navigationText(void);
    bool set_navigationText(const std::string& navigationText_);
    bool set_turnIcon(const Image& turnIcon_);

  private:

    friend class TurnMarshaller;

      std::string* navigationText;	//!< (500)
      Image turnIcon;
  };

}

#endif
