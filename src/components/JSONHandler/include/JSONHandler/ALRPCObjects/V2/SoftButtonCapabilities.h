#ifndef NSAPPLINKRPCV2_SOFTBUTTONCAPABILITIES_INCLUDE
#define NSAPPLINKRPCV2_SOFTBUTTONCAPABILITIES_INCLUDE




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

///  Contains information about a SoftButton's capabilities.

  class SoftButtonCapabilities
  {
  public:
  
    SoftButtonCapabilities(const SoftButtonCapabilities& c);
    SoftButtonCapabilities(void);
  
    bool checkIntegrity(void);
  // getters

    bool get_imageSupported(void) const;
    bool get_longPressAvailable(void) const;
    bool get_shortPressAvailable(void) const;
    bool get_upDownAvailable(void) const;

// setters

    bool set_imageSupported(bool imageSupported_);
    bool set_longPressAvailable(bool longPressAvailable_);
    bool set_shortPressAvailable(bool shortPressAvailable_);
    bool set_upDownAvailable(bool upDownAvailable_);

  private:

    friend class SoftButtonCapabilitiesMarshaller;


///  The button supports referencing a static or dynamic image.
      bool imageSupported;

/**
     The button supports a LONG press.
     Whenever the button is pressed long, onButtonPressed( LONG) will be invoked.
*/
      bool longPressAvailable;

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
