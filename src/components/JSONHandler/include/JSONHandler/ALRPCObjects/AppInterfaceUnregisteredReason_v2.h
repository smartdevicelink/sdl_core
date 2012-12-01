#ifndef NSAPPLINKRPC_APPINTERFACEUNREGISTEREDREASON_V2_INCLUDE
#define NSAPPLINKRPC_APPINTERFACEUNREGISTEREDREASON_V2_INCLUDE


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

///  Error code, which comes from sync side.

  class AppInterfaceUnregisteredReason_v2
  {
  public:
    enum AppInterfaceUnregisteredReason_v2Internal
    {
      INVALID_ENUM=-1,
      USER_EXIT=0,
      IGNITION_OFF=1,
      BLUETOOTH_OFF=2,
      USB_DISCONNECTED=3,
      REQUEST_WHILE_IN_NONE_HMI_LEVEL=4,
      TOO_MANY_REQUESTS=5,
      DRIVER_DISTRACTION_VIOLATION=6,
      LANGUAGE_CHANGE=7,
      MASTER_RESET=8,
      FACTORY_DEFAULTS=9,
      APP_UNAUTHORIZED=10
    };
  
    AppInterfaceUnregisteredReason_v2() : mInternal(INVALID_ENUM)				{}
    AppInterfaceUnregisteredReason_v2(AppInterfaceUnregisteredReason_v2Internal e) : mInternal(e)		{}
  
    AppInterfaceUnregisteredReason_v2Internal get(void) const	{ return mInternal; }
    void set(AppInterfaceUnregisteredReason_v2Internal e)		{ mInternal=e; }
  
  private:
    AppInterfaceUnregisteredReason_v2Internal mInternal;
    friend class AppInterfaceUnregisteredReason_v2Marshaller;
  };
  
}

#endif
