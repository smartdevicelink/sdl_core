#ifndef NSAPPLINKRPCV2_ONPERMISSIONSCHANGE_INCLUDE
#define NSAPPLINKRPCV2_ONPERMISSIONSCHANGE_INCLUDE

#include <vector>

#include "PermissionItem.h"
#include "JSONHandler/ALRPC2Message.hh"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

///  Provides update to app of which policy-table-enabled functions are available

  class OnPermissionsChange : public ALRPC2Message
  {
  public:
  
    OnPermissionsChange(const OnPermissionsChange& c);
    OnPermissionsChange(void);
    
    virtual ~OnPermissionsChange(void);
  
    bool checkIntegrity(void);

    const std::vector<PermissionItem>& get_permissionItem(void) const;

    bool set_permissionItem(const std::vector<PermissionItem>& permissionItem_);

  private:
  
    friend class OnPermissionsChangeMarshaller;


///  Change in permissions for a given set of RPCs.
      std::vector<PermissionItem> permissionItem;	//!<   [%s..%s] 
  };

}

#endif
