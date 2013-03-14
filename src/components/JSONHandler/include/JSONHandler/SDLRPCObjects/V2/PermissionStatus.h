#ifndef NSAPPLINKRPCV2_PERMISSIONSTATUS_INCLUDE
#define NSAPPLINKRPCV2_PERMISSIONSTATUS_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
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
