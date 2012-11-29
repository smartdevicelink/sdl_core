#include "../include/JSONHandler/ALRPCObjects/HMIPermissions.h"
#include "HMIPermissionsMarshaller.h"
#include "HMILevelMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

HMIPermissions::HMIPermissions(const HMIPermissions& c)
{
  *this=c;
}


bool HMIPermissions::checkIntegrity(void)
{
  return HMIPermissionsMarshaller::checkIntegrity(*this);
}


HMIPermissions::HMIPermissions(void)
{
}



bool HMIPermissions::set_allowed(const std::vector<HMILevel>& allowed_)
{
  unsigned int i=allowed_.size();
  if(i>100 || i<0)  return false;
  while(i--)
  {
    if(!HMILevelMarshaller::checkIntegrityConst(allowed_[i]))   return false;
  }
  allowed=allowed_;
  return true;
}

bool HMIPermissions::set_userDisallowed(const std::vector<HMILevel>& userDisallowed_)
{
  unsigned int i=userDisallowed_.size();
  if(i>100 || i<0)  return false;
  while(i--)
  {
    if(!HMILevelMarshaller::checkIntegrityConst(userDisallowed_[i]))   return false;
  }
  userDisallowed=userDisallowed_;
  return true;
}




const std::vector<HMILevel>& HMIPermissions::get_allowed(void) const 
{
  return allowed;
}


const std::vector<HMILevel>& HMIPermissions::get_userDisallowed(void) const 
{
  return userDisallowed;
}


