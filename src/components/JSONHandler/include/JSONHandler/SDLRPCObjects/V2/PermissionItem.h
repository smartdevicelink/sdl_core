//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKKRPCV2_PERMISSIONITEM_INCLUDE
#define NSSMARTDEVICELINKKRPCV2_PERMISSIONITEM_INCLUDE

#include <string>

#include "HMIPermissions.h"
#include "ParameterPermissions.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/


namespace NsSmartDeviceLinkRPCV2
{

  class PermissionItem
  {
  public:
  
    PermissionItem(const PermissionItem& c);
    PermissionItem(void);
  
    bool checkIntegrity(void);
  // getters

    const HMIPermissions& get_hmiPermissions(void) const;
    const ParameterPermissions& get_parameterPermissions(void) const;
    const std::string& get_rpcName(void) const;

// setters

    bool set_hmiPermissions(const HMIPermissions& hmiPermissions_);
    bool set_parameterPermissions(const ParameterPermissions& parameterPermissions_);
    bool set_rpcName(const std::string& rpcName_);

  private:

    friend class PermissionItemMarshaller;

      HMIPermissions hmiPermissions;
      ParameterPermissions parameterPermissions;

///  Name of the individual RPC in the policy table.
      std::string rpcName;	//!< (100)
  };

}

#endif
