#ifndef PERMISSIONSTATUS_INCLUDE
#define PERMISSIONSTATUS_INCLUDE


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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

#endif
