#ifndef APPINTERFACEUNREGISTEREDREASON_INCLUDE
#define APPINTERFACEUNREGISTEREDREASON_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/


///  Error code, which comes from sync side.

class AppInterfaceUnregisteredReason
{
public:
  enum AppInterfaceUnregisteredReasonInternal
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

  AppInterfaceUnregisteredReason() : mInternal(INVALID_ENUM)				{}
  AppInterfaceUnregisteredReason(AppInterfaceUnregisteredReasonInternal e) : mInternal(e)		{}

  AppInterfaceUnregisteredReasonInternal get(void) const	{ return mInternal; }
  void set(AppInterfaceUnregisteredReasonInternal e)		{ mInternal=e; }

private:
  AppInterfaceUnregisteredReasonInternal mInternal;
  friend class AppInterfaceUnregisteredReasonMarshaller;
};

#endif
