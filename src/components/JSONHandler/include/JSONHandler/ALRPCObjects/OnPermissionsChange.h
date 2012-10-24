#ifndef ONPERMISSIONSCHANGE_INCLUDE
#define ONPERMISSIONSCHANGE_INCLUDE

#include <vector>

#include "PermissionGroupItem.h"
#include "../../../JSONHandler/ALRPCNotification.hh"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


///  Provides update to app of which policy-table-enabled functions are available

class OnPermissionsChange : public ALRPCNotification
{
public:

  OnPermissionsChange(const OnPermissionsChange& c);
  OnPermissionsChange(void);
  
  virtual ~OnPermissionsChange(void);

  bool checkIntegrity(void);

  const std::vector<PermissionGroupItem>& get_PermissionGroupItems(void) const;

  bool set_PermissionGroupItems(const std::vector<PermissionGroupItem>& PermissionGroupItems_);

private:

  friend class OnPermissionsChangeMarshaller;

  std::vector<PermissionGroupItem> PermissionGroupItems;	//!<   [%s..%s] 
};

#endif
