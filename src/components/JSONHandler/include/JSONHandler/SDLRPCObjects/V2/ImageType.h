//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_IMAGETYPE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_IMAGETYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
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
