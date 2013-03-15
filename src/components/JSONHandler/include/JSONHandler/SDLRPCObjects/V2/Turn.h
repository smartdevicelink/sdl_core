//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_TURN_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_TURN_INCLUDE

#include <string>

#include "Image.h"


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
