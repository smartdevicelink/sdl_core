#ifndef NSAPPLINKRPCV2_IMAGE_INCLUDE
#define NSAPPLINKRPCV2_IMAGE_INCLUDE

#include <string>

#include "ImageType.h"


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

  class Image
  {
  public:
  
    Image(const Image& c);
    Image(void);
  
    bool checkIntegrity(void);
  // getters

    const ImageType& get_imageType(void) const;
    const std::string& get_value(void) const;

// setters

    bool set_imageType(const ImageType& imageType_);
    bool set_value(const std::string& value_);

  private:

    friend class ImageMarshaller;


///  Describes, whether it is a static or dynamic image.
      ImageType imageType;

///  Either the static hex icon value or the binary image file name identifier (sent by PutFile).
      std::string value;	//!< (65535)
  };

}

#endif
