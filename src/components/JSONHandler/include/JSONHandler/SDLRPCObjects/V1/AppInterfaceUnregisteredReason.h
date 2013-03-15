//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPC_APPINTERFACEUNREGISTEREDREASON_INCLUDE
#define NSSMARTDEVICELINKKRPC_APPINTERFACEUNREGISTEREDREASON_INCLUDE


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
{

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
  
}

#endif
