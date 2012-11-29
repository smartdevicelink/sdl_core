#ifndef NSAPPLINKRPC_HMIPERMISSIONS_INCLUDE
#define NSAPPLINKRPC_HMIPERMISSIONS_INCLUDE

#include <vector>

#include "HMILevel.h"
#include "HMILevel.h"


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
