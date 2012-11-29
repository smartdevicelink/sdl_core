#ifndef NSAPPLINKRPC_PERMISSIONITEM_INCLUDE
#define NSAPPLINKRPC_PERMISSIONITEM_INCLUDE

#include <string>

#include "HMIPermissions.h"


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

  class PermissionItem
  {
  public:
  
    PermissionItem(const PermissionItem& c);
    PermissionItem(void);
  
    bool checkIntegrity(void);
  // getters

    const HMIPermissions& get_hmiPermissions(void) const;
    const std::string& get_rpcName(void) const;

// setters

    bool set_hmiPermissions(const HMIPermissions& hmiPermissions_);
    bool set_rpcName(const std::string& rpcName_);

  private:

    friend class PermissionItemMarshaller;

      HMIPermissions hmiPermissions;

///  Name of the individual RPC in the policy table.
      std::string rpcName;	//!< (100)
  };

}

#endif
