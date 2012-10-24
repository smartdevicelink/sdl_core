#ifndef HMILEVEL_INCLUDE
#define HMILEVEL_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Oct 24 15:41:28 2012
  source stamp	Wed Oct 24 14:57:16 2012
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
