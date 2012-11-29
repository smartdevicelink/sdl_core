#ifndef NSAPPLINKRPC_SYSTEMCONTEXT_V2_INCLUDE
#define NSAPPLINKRPC_SYSTEMCONTEXT_V2_INCLUDE


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

/**
     Enumeration that describes possible contexts an app's HMI might be in.
     Communicated to whichever app is in HMI FULL, except Alert.
*/

  class SystemContext_v2
  {
  public:
    enum SystemContext_v2Internal
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
      SYSCTXT_ALERT=4
    };
  
    SystemContext_v2() : mInternal(INVALID_ENUM)				{}
    SystemContext_v2(SystemContext_v2Internal e) : mInternal(e)		{}
  
    SystemContext_v2Internal get(void) const	{ return mInternal; }
    void set(SystemContext_v2Internal e)		{ mInternal=e; }
  
  private:
    SystemContext_v2Internal mInternal;
    friend class SystemContext_v2Marshaller;
  };
  
}

#endif
