#ifndef NSAPPLINKRPCV2_HMIPERMISSIONS_INCLUDE
#define NSAPPLINKRPCV2_HMIPERMISSIONS_INCLUDE

#include <vector>

#include "HMILevel.h"
#include "HMILevel.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/


namespace NsAppLinkRPCV2
{

  class HMIPermissions
  {
  public:
  
    HMIPermissions(const HMIPermissions& c);
    HMIPermissions(void);
  
    bool checkIntegrity(void);
  // getters

    const std::vector<HMILevel>& get_allowed(void) const;
    const std::vector<HMILevel>& get_userDisallowed(void) const;

// setters

    bool set_allowed(const std::vector<HMILevel>& allowed_);
    bool set_userDisallowed(const std::vector<HMILevel>& userDisallowed_);

  private:

    friend class HMIPermissionsMarshaller;


///  A set of all HMI levels that are permitted for this given RPC.
      std::vector<HMILevel> allowed;	//!<   [%s..%s] 

///  A set of all HMI levels that are prohibited for this given RPC.
      std::vector<HMILevel> userDisallowed;	//!<   [%s..%s] 
  };

}

#endif
