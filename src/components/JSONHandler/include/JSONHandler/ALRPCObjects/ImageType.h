#ifndef IMAGETYPE_INCLUDE
#define IMAGETYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Contains information about the type of image.

class ImageType
{
public:
  enum ImageTypeInternal
  {
    INVALID_ENUM=-1,
    STATIC=0,
    DYNAMIC=1
  };

  ImageType() : mInternal(INVALID_ENUM)				{}
  ImageType(ImageTypeInternal e) : mInternal(e)		{}

  ImageTypeInternal get(void) const	{ return mInternal; }
  void set(ImageTypeInternal e)		{ mInternal=e; }

private:
  ImageTypeInternal mInternal;
  friend class ImageTypeMarshaller;
};

#endif
