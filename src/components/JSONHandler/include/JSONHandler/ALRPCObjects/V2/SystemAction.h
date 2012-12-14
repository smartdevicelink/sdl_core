#ifndef NSAPPLINKRPCV2_SYSTEMACTION_INCLUDE
#define NSAPPLINKRPCV2_SYSTEMACTION_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Enumeration that describes system actions that can be triggered.

  class SystemAction
  {
  public:
    enum SystemActionInternal
    {
      INVALID_ENUM=-1,

///  Default action occurs.  Standard behavior (e.g. SoftButton clears overlay).
      DEFAULT_ACTION=0,

///  App is brought into HMI_FULL.
      STEAL_FOCUS=1,

///  Current system context is maintained.  An overlay is persisted even though a SoftButton has been pressed and the notification sent.
      KEEP_CONTEXT=2
    };
  
    SystemAction() : mInternal(INVALID_ENUM)				{}
    SystemAction(SystemActionInternal e) : mInternal(e)		{}
  
    SystemActionInternal get(void) const	{ return mInternal; }
    void set(SystemActionInternal e)		{ mInternal=e; }
  
  private:
    SystemActionInternal mInternal;
    friend class SystemActionMarshaller;
  };
  
}

#endif
