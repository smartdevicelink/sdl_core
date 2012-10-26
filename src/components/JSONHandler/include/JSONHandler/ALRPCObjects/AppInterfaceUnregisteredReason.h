#ifndef APPINTERFACEUNREGISTEREDREASON_INCLUDE
#define APPINTERFACEUNREGISTEREDREASON_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
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
    FACTORY_DEFAULTS=9
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
