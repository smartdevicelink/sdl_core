#ifndef PERMISSIONGROUPITEM_INCLUDE
#define PERMISSIONGROUPITEM_INCLUDE

#include <string>

#include "PermissionStatus.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



class PermissionGroupItem
{
public:

  PermissionGroupItem(const PermissionGroupItem& c);
  PermissionGroupItem(void);

  bool checkIntegrity(void);
// getters

  const std::string& get_PermissionGroupName(void) const;
  const PermissionStatus& get_PermissionGroupStatus(void) const;

// setters

  bool set_PermissionGroupName(const std::string& PermissionGroupName_);
  bool set_PermissionGroupStatus(const PermissionStatus& PermissionGroupStatus_);

private:

  friend class PermissionGroupItemMarshaller;

  std::string PermissionGroupName;	//!< (100)
  PermissionStatus PermissionGroupStatus;
};

#endif
