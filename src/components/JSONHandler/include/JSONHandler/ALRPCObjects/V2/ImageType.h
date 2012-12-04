#ifndef NSAPPLINKRPCV2_IMAGETYPE_INCLUDE
#define NSAPPLINKRPCV2_IMAGETYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

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
  
}

#endif
