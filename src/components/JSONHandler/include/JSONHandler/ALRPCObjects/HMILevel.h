#ifndef HMILEVEL_INCLUDE
#define HMILEVEL_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Enumeraction that describes current levels of HMI.

class HMILevel
{
public:
  enum HMILevelInternal
  {
    INVALID_ENUM=-1,
    HMI_FULL=0,
    HMI_LIMITED=1,
    HMI_BACKGROUND=2,
    HMI_NONE=3
  };

  HMILevel() : mInternal(INVALID_ENUM)				{}
  HMILevel(HMILevelInternal e) : mInternal(e)		{}

  HMILevelInternal get(void) const	{ return mInternal; }
  void set(HMILevelInternal e)		{ mInternal=e; }

private:
  HMILevelInternal mInternal;
  friend class HMILevelMarshaller;
};

#endif
