#include "../../include/JSONHandler/ALRPCObjects/PermissionGroupItem.h"
#include "PermissionGroupItemMarshaller.h"
#include "PermissionStatusMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



PermissionGroupItem::PermissionGroupItem(const PermissionGroupItem& c)
{
  *this=c;
}


bool PermissionGroupItem::checkIntegrity(void)
{
  return PermissionGroupItemMarshaller::checkIntegrity(*this);
}


PermissionGroupItem::PermissionGroupItem(void)
{
}



bool PermissionGroupItem::set_PermissionGroupName(const std::string& PermissionGroupName_)
{
  if(PermissionGroupName_.length()>100)  return false;
  PermissionGroupName=PermissionGroupName_;
  return true;
}

bool PermissionGroupItem::set_PermissionGroupStatus(const PermissionStatus& PermissionGroupStatus_)
{
  if(!PermissionStatusMarshaller::checkIntegrityConst(PermissionGroupStatus_))   return false;
  PermissionGroupStatus=PermissionGroupStatus_;
  return true;
}




const std::string& PermissionGroupItem::get_PermissionGroupName(void) const 
{
  return PermissionGroupName;
}


const PermissionStatus& PermissionGroupItem::get_PermissionGroupStatus(void) const 
{
  return PermissionGroupStatus;
}


