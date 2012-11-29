#ifndef NSAPPLINKRPC_IMAGE_INCLUDE
#define NSAPPLINKRPC_IMAGE_INCLUDE

#include <string>

#include "ImageType.h"


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
