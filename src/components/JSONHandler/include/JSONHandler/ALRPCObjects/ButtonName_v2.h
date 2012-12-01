#ifndef NSAPPLINKRPC_BUTTONNAME_V2_INCLUDE
#define NSAPPLINKRPC_BUTTONNAME_V2_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

///  Defines the hard (physical) and soft (touchscreen) buttons available from SYNC

  class ButtonName_v2
  {
  public:
    enum ButtonName_v2Internal
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
      PRESET_9=14,
      CUSTOM_BUTTON=15
    };
  
    ButtonName_v2() : mInternal(INVALID_ENUM)				{}
    ButtonName_v2(ButtonName_v2Internal e) : mInternal(e)		{}
  
    ButtonName_v2Internal get(void) const	{ return mInternal; }
    void set(ButtonName_v2Internal e)		{ mInternal=e; }
  
  private:
    ButtonName_v2Internal mInternal;
    friend class ButtonName_v2Marshaller;
  };
  
}

#endif
