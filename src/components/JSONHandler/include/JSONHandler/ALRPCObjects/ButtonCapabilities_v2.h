#ifndef NSAPPLINKRPC_BUTTONCAPABILITIES_V2_INCLUDE
#define NSAPPLINKRPC_BUTTONCAPABILITIES_V2_INCLUDE


#include "ButtonName_v2.h"


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

///  Contains information about a button's capabilities.

  class ButtonCapabilities_v2
  {
  public:
  
    ButtonCapabilities_v2(const ButtonCapabilities_v2& c);
    ButtonCapabilities_v2(void);
  
    bool checkIntegrity(void);
  // getters

    bool get_longPressAvailable(void) const;
    const ButtonName_v2& get_name(void) const;
    bool get_shortPressAvailable(void) const;
    bool get_upDownAvailable(void) const;

// setters

    bool set_longPressAvailable(bool longPressAvailable_);
    bool set_name(const ButtonName_v2& name_);
    bool set_shortPressAvailable(bool shortPressAvailable_);
    bool set_upDownAvailable(bool upDownAvailable_);

  private:

    friend class ButtonCapabilities_v2Marshaller;


/**
     The button supports a LONG press.
     Whenever the button is pressed long, onButtonPressed( LONG) will be invoked.
*/
      bool longPressAvailable;

///  The name of the button. See ButtonName.
      ButtonName_v2 name;

/**
     The button supports a short press.
     Whenever the button is pressed short, onButtonPressed( SHORT) will be invoked.
*/
      bool shortPressAvailable;

/**
     The button supports "button down" and "button up".
     Whenever the button is pressed, onButtonEvent( DOWN) will be invoked.
     Whenever the button is released, onButtonEvent( UP) will be invoked.
*/
      bool upDownAvailable;
  };

}

#endif
