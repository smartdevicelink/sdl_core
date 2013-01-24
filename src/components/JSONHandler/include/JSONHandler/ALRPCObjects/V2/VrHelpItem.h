#ifndef NSAPPLINKRPCV2_VRHELPITEM_INCLUDE
#define NSAPPLINKRPCV2_VRHELPITEM_INCLUDE

#include <string>

#include "Image.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

  class VrHelpItem
  {
  public:
  
    VrHelpItem(const VrHelpItem& c);
    VrHelpItem(void);
  
    bool checkIntegrity(void);
  // getters

    const Image& get_image(void) const;
    unsigned int get_position(void) const;
    const std::string& get_text(void) const;

// setters

    bool set_image(const Image& image_);
    bool set_position(unsigned int position_);
    bool set_text(const std::string& text_);

  private:

    friend class VrHelpItemMarshaller;


///  Image struct for VR Help item
      Image image;

///  Position to display item in VR Help list
      unsigned int position;	//!<  (1,8)

///  Text to display for VR Help item
      std::string text;	//!< (500)
  };

}

#endif
