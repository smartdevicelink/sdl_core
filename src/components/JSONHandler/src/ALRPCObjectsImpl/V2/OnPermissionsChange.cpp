#include "../include/JSONHandler/ALRPCObjects/V2/OnPermissionsChange.h"
#include "OnPermissionsChangeMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "PermissionItemMarshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

OnPermissionsChange::~OnPermissionsChange(void)
{
}


OnPermissionsChange::OnPermissionsChange(const OnPermissionsChange& c) : ALRPC2Message(c)
{
  *this=c;
}


bool OnPermissionsChange::checkIntegrity(void)
{
  return OnPermissionsChangeMarshaller::checkIntegrity(*this);
}


OnPermissionsChange::OnPermissionsChange(void) : ALRPC2Message(PROTOCOL_VERSION)
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

