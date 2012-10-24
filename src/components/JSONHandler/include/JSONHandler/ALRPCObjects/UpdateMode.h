#ifndef UPDATEMODE_INCLUDE
#define UPDATEMODE_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


/**
     Describes how the media clock timer should behave on the platform
     Starts the media clock timer counting upwards, as in time elapsed.
     Starts the media clock timer counting downwards, as in time remaining.
     Pauses the media clock timer
     Resume the media clock timer
     Clears the media clock timer (previously done through Show->mediaClock
*/

class UpdateMode
{
public:
  enum UpdateModeInternal
  {
    INVALID_ENUM=-1,
    COUNTUP=0,
    COUNTDOWN=1,
    PAUSE=2,
    RESUME=3,
    CLEAR=4
  };

  UpdateMode() : mInternal(INVALID_ENUM)				{}
  UpdateMode(UpdateModeInternal e) : mInternal(e)		{}

  UpdateModeInternal get(void) const	{ return mInternal; }
  void set(UpdateModeInternal e)		{ mInternal=e; }

private:
  UpdateModeInternal mInternal;
  friend class UpdateModeMarshaller;
};

#endif
