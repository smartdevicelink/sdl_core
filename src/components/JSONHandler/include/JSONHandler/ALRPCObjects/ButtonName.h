#ifndef BUTTONNAME_INCLUDE
#define BUTTONNAME_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


///  Defines the hard (physical) and soft (touchscreen) buttons available from SYNC

class ButtonName
{
public:
  enum ButtonNameInternal
  {
    INVALID_ENUM=-1,
    OK=0,
    SEEKLEFT=1,
    SEEKRIGHT=2,
    TUNEUP=3,
    TUNEDOWN=4,
    PRESET_0=5,
    PRESET_1=6,
    PRESET_2=7,
    PRESET_3=8,
    PRESET_4=9,
    PRESET_5=10,
    PRESET_6=11,
    PRESET_7=12,
    PRESET_8=13,
    PRESET_9=14
  };

  ButtonName() : mInternal(INVALID_ENUM)				{}
  ButtonName(ButtonNameInternal e) : mInternal(e)		{}

  ButtonNameInternal get(void) const	{ return mInternal; }
  void set(ButtonNameInternal e)		{ mInternal=e; }

private:
  ButtonNameInternal mInternal;
  friend class ButtonNameMarshaller;
};

#endif
