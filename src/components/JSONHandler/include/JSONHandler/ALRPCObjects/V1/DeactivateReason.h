#ifndef NSAPPLINKRPC_DEACTIVATEREASON_INCLUDE
#define NSAPPLINKRPC_DEACTIVATEREASON_INCLUDE


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

///  Enum of reasons of user navigating from application used by HMI.

  class DeactivateReason
  {
  public:
    enum DeactivateReasonInternal
    {
      INVALID_ENUM=-1,

///  Navigated to audion(radio, etc)
      AUDIO=0,

///  Navigated to make a call.
      PHONECALL=1,

///  Navigated to navigation screen.
      NAVIGATIONMAP=2,

///  Navigated to phone menu.
      PHONEMENU=3,

///  Navigated to settings menu.
      SYNCSETTINGS=4,

///  Other screens navigation apart from other mobile app.
      GENERAL=5
    };
  
    DeactivateReason() : mInternal(INVALID_ENUM)				{}
    DeactivateReason(DeactivateReasonInternal e) : mInternal(e)		{}
  
    DeactivateReasonInternal get(void) const	{ return mInternal; }
    void set(DeactivateReasonInternal e)		{ mInternal=e; }
  
  private:
    DeactivateReasonInternal mInternal;
    friend class DeactivateReasonMarshaller;
  };
  
}

#endif
