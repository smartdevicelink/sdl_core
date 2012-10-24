#ifndef SOFTBUTTONTYPE_INCLUDE
#define SOFTBUTTONTYPE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Contains information about the SoftButton capabilities.

class SoftButtonType
{
public:
  enum SoftButtonTypeInternal
  {
    INVALID_ENUM=-1,
    TEXT=0,
    IMAGE=1,
    BOTH=2
  };

  SoftButtonType() : mInternal(INVALID_ENUM)				{}
  SoftButtonType(SoftButtonTypeInternal e) : mInternal(e)		{}

  SoftButtonTypeInternal get(void) const	{ return mInternal; }
  void set(SoftButtonTypeInternal e)		{ mInternal=e; }

private:
  SoftButtonTypeInternal mInternal;
  friend class SoftButtonTypeMarshaller;
};

#endif
