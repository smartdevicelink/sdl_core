#include "../../include/JSONHandler/ALRPCObjects/OnPermissionsChange.h"
#include "OnPermissionsChangeMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "PermissionGroupItemMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


OnPermissionsChange::~OnPermissionsChange(void)
{
}


OnPermissionsChange::OnPermissionsChange(const OnPermissionsChange& c)
{
  *this=c;
}


bool OnPermissionsChange::checkIntegrity(void)
{
  return OnPermissionsChangeMarshaller::checkIntegrity(*this);
}


OnPermissionsChange::OnPermissionsChange(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONPERMISSIONSCHANGE)
{
}



bool OnPermissionsChange::set_PermissionGroupItems(const std::vector<PermissionGroupItem>& PermissionGroupItems_)
{
  unsigned int i=PermissionGroupItems_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!PermissionGroupItemMarshaller::checkIntegrityConst(PermissionGroupItems_[i]))   return false;
  }
  PermissionGroupItems=PermissionGroupItems_;
  return true;
}




const std::vector<PermissionGroupItem>& OnPermissionsChange::get_PermissionGroupItems(void) const 
{
  return PermissionGroupItems;
}

