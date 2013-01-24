#include "../include/JSONHandler/ALRPCObjects/V2/PermissionItem.h"
#include "PermissionItemMarshaller.h"
#include "HMIPermissionsMarshaller.h"
#include "ParameterPermissionsMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

PermissionItem::PermissionItem(const PermissionItem& c)
{
  *this=c;
}


bool PermissionItem::checkIntegrity(void)
{
  return PermissionItemMarshaller::checkIntegrity(*this);
}


PermissionItem::PermissionItem(void)
{
}



bool PermissionItem::set_hmiPermissions(const HMIPermissions& hmiPermissions_)
{
  if(!HMIPermissionsMarshaller::checkIntegrityConst(hmiPermissions_))   return false;
  hmiPermissions=hmiPermissions_;
  return true;
}

bool PermissionItem::set_parameterPermissions(const ParameterPermissions& parameterPermissions_)
{
  if(!ParameterPermissionsMarshaller::checkIntegrityConst(parameterPermissions_))   return false;
  parameterPermissions=parameterPermissions_;
  return true;
}

bool PermissionItem::set_rpcName(const std::string& rpcName_)
{
  if(rpcName_.length()>100)  return false;
  rpcName=rpcName_;
  return true;
}




const HMIPermissions& PermissionItem::get_hmiPermissions(void) const 
{
  return hmiPermissions;
}


const ParameterPermissions& PermissionItem::get_parameterPermissions(void) const 
{
  return parameterPermissions;
}


const std::string& PermissionItem::get_rpcName(void) const 
{
  return rpcName;
}


