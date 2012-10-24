#ifndef IMAGE_INCLUDE
#define IMAGE_INCLUDE

#include <string>

#include "ImageType.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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

  ImageType imageType;
  std::string value;	//!< (65535)
};

#endif
