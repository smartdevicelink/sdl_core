#ifndef NSAPPLINKRPC_SYSTEMACTION_INCLUDE
#define NSAPPLINKRPC_SYSTEMACTION_INCLUDE


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
