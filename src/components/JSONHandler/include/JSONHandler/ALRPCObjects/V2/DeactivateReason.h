#ifndef NSAPPLINKRPCV2_DEACTIVATEREASON_INCLUDE
#define NSAPPLINKRPCV2_DEACTIVATEREASON_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Enum of reasons of user navigating from application used by HMI.

  class DeactivateReason
  {
  public:
    enum DeactivateReasonInternal
    {
      INVALID_ENUM=-1,

///  Navigated to audio(radio, etc)
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
