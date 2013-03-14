#ifndef NSAPPLINKRPCV2_ONPERMISSIONSCHANGE_INCLUDE
#define NSAPPLINKRPCV2_ONPERMISSIONSCHANGE_INCLUDE

#include <vector>

#include "PermissionItem.h"
#include "JSONHandler/SDLRPCMessage.h"


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

///  Provides update to app of which policy-table-enabled functions are available

  class OnPermissionsChange : public NsSmartDeviceLinkRPC::SDLRPCMessage
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
