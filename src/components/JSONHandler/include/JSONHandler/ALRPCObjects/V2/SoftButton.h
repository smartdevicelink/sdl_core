#ifndef NSAPPLINKRPCV2_SOFTBUTTON_INCLUDE
#define NSAPPLINKRPCV2_SOFTBUTTON_INCLUDE

#include <string>

#include "Image.h"
#include "SystemAction.h"
#include "SoftButtonType.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

  class SoftButton
  {
  public:
  
    SoftButton(const SoftButton& c);
    SoftButton(void);
  
    bool checkIntegrity(void);
  // getters

    const Image& get_image(void) const;
    bool get_isHighlighted(void) const;
    unsigned int get_softButtonID(void) const;
    const SystemAction& get_systemAction(void) const;
    const std::string& get_text(void) const;
    const SoftButtonType& get_type(void) const;

// setters

    bool set_image(const Image& image_);
    bool set_isHighlighted(bool isHighlighted_);
    bool set_softButtonID(unsigned int softButtonID_);
    bool set_systemAction(const SystemAction& systemAction_);
    bool set_text(const std::string& text_);
    bool set_type(const SoftButtonType& type_);

  private:

    friend class SoftButtonMarshaller;


///  Optional image struct for SoftButton (if defined as IMAGE or BOTH)
      Image image;

/**
     True, if highlighted
     False, if not highlighted
*/
      bool isHighlighted;

///  Value which is returned via OnButtonPress / OnButtonEvent
      unsigned int softButtonID;	//!<  (0,65536)

///  Parameter indicating whether selecting a SoftButton shall call a specific system action.  This is intended to allow Notifications to bring the callee into full / focus; or in the case of persistent overlays, the overlay can persist when a SoftButton is pressed.
      SystemAction systemAction;

///  Optional text to display (if defined as TEXT or BOTH)
      std::string text;	//!< (500)

///  Describes, whether it is text, highlighted text, icon, or dynamic image. See softButtonType
      SoftButtonType type;
  };

}

#endif
