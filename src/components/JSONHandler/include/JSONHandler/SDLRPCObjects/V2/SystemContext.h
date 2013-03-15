//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_SYSTEMCONTEXT_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_SYSTEMCONTEXT_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

/**
     Enumeration that describes possible contexts an app's HMI might be in.
     Communicated to whichever app is in HMI FULL, except Alert.
*/

  class SystemContext
  {
  public:
    enum SystemContextInternal
    {
      INVALID_ENUM=-1,

///  The app's persistent display (whether media/non-media/navigation) is fully visible onscreen.
      SYSCTXT_MAIN=0,

///  The system is currently in a VR session (with whatever dedicated VR screen being overlaid onscreen).
      SYSCTXT_VRSESSION=1,

///  The system is currently displaying a system or in-App menu onscreen.
      SYSCTXT_MENU=2,

///  The app's display HMI is currently being obscured by either a system or other app's overlay.
      SYSCTXT_HMI_OBSCURED=3,

///  Broadcast only to whichever app has an alert currently being displayed.
      SYSCTXT_ALERT=4,
      NOT_AUDIBLE=5,
      AUDIBLE=6
    };

    SystemContext() : mInternal(INVALID_ENUM)				{}
    SystemContext(SystemContextInternal e) : mInternal(e)		{}

    SystemContextInternal get(void) const	{ return mInternal; }
    void set(SystemContextInternal e)		{ mInternal=e; }

  private:
    SystemContextInternal mInternal;
    friend class SystemContextMarshaller;
  };

}

#endif
