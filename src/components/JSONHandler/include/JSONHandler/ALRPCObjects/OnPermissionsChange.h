#ifndef NSAPPLINKRPC_ONPERMISSIONSCHANGE_INCLUDE
#define NSAPPLINKRPC_ONPERMISSIONSCHANGE_INCLUDE

#include <vector>

#include "PermissionItem.h"
#include "JSONHandler/ALRPCNotification.h"


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

///  Provides update to app of which policy-table-enabled functions are available

  class OnPermissionsChange : public ALRPCNotification
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
