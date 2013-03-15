//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_TEXTALIGNMENT_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_TEXTALIGNMENT_INCLUDE


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

///  The list of possible alignments, left, right, or centered

  class TextAlignment
  {
  public:
    enum TextAlignmentInternal
    {
      INVALID_ENUM=-1,
      LEFT_ALIGNED=0,
      RIGHT_ALIGNED=1,
      CENTERED=2
    };
  
    TextAlignment() : mInternal(INVALID_ENUM)				{}
    TextAlignment(TextAlignmentInternal e) : mInternal(e)		{}
  
    TextAlignmentInternal get(void) const	{ return mInternal; }
    void set(TextAlignmentInternal e)		{ mInternal=e; }
  
  private:
    TextAlignmentInternal mInternal;
    friend class TextAlignmentMarshaller;
  };
  
}

#endif
