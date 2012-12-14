#ifndef NSAPPLINKRPC_BUTTONNAME_INCLUDE
#define NSAPPLINKRPC_BUTTONNAME_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

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
  
}

#endif
