#ifndef NSAPPLINKRPCV2_PERMISSIONITEM_INCLUDE
#define NSAPPLINKRPCV2_PERMISSIONITEM_INCLUDE

#include <string>

#include "HMIPermissions.h"


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
