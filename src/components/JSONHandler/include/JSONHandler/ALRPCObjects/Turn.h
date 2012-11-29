#ifndef NSAPPLINKRPC_TURN_INCLUDE
#define NSAPPLINKRPC_TURN_INCLUDE

#include <string>

#include "Image.h"


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
