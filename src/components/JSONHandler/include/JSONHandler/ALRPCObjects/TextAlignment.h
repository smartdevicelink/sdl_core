#ifndef TEXTALIGNMENT_INCLUDE
#define TEXTALIGNMENT_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


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

#endif
