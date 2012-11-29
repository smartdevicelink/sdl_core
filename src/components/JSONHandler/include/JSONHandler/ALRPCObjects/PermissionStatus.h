#ifndef NSAPPLINKRPC_PERMISSIONSTATUS_INCLUDE
#define NSAPPLINKRPC_PERMISSIONSTATUS_INCLUDE


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

///  Enumeration that describes possible permission states of a policy table entry.

  class PermissionStatus
  {
  public:
    enum PermissionStatusInternal
    {
      INVALID_ENUM=-1,
      PS_ALLOWED=0,
      PS_DISALLOWED=1,
      PS_USER_DISALLOWED=2,
      PS_USER_CONSENT_PENDING=3
    };
  
    PermissionStatus() : mInternal(INVALID_ENUM)				{}
    PermissionStatus(PermissionStatusInternal e) : mInternal(e)		{}
  
    PermissionStatusInternal get(void) const	{ return mInternal; }
    void set(PermissionStatusInternal e)		{ mInternal=e; }
  
  private:
    PermissionStatusInternal mInternal;
    friend class PermissionStatusMarshaller;
  };
  
}

#endif
