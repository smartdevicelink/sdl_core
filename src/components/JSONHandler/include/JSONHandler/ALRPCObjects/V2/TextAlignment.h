#ifndef NSAPPLINKRPCV2_TEXTALIGNMENT_INCLUDE
#define NSAPPLINKRPCV2_TEXTALIGNMENT_INCLUDE


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
