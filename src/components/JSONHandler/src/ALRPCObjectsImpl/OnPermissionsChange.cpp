#include "../include/JSONHandler/ALRPCObjects/OnPermissionsChange.h"
#include "OnPermissionsChangeMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "PermissionItemMarshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

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



bool OnPermissionsChange::set_permissionItem(const std::vector<PermissionItem>& permissionItem_)
{
  unsigned int i=permissionItem_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(!PermissionItemMarshaller::checkIntegrityConst(permissionItem_[i]))   return false;
  }
  permissionItem=permissionItem_;
  return true;
}




const std::vector<PermissionItem>& OnPermissionsChange::get_permissionItem(void) const 
{
  return permissionItem;
}

