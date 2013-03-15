//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SOFTBUTTONTYPE_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SOFTBUTTONTYPE_INCLUDE


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

///  Contains information about the SoftButton capabilities.

  class SoftButtonType
  {
  public:
    enum SoftButtonTypeInternal
    {
      INVALID_ENUM=-1,
      SBT_TEXT=0,
      SBT_IMAGE=1,
      SBT_BOTH=2
    };
  
    SoftButtonType() : mInternal(INVALID_ENUM)				{}
    SoftButtonType(SoftButtonTypeInternal e) : mInternal(e)		{}
  
    SoftButtonTypeInternal get(void) const	{ return mInternal; }
    void set(SoftButtonTypeInternal e)		{ mInternal=e; }
  
  private:
    SoftButtonTypeInternal mInternal;
    friend class SoftButtonTypeMarshaller;
  };
  
}

#endif
