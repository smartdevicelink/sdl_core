#ifndef NSAPPLINKRPC_IMAGETYPE_INCLUDE
#define NSAPPLINKRPC_IMAGETYPE_INCLUDE


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
