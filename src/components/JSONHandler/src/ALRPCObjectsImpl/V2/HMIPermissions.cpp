#include "../include/JSONHandler/ALRPCObjects/V2/HMIPermissions.h"
#include "HMIPermissionsMarshaller.h"
#include "HMILevelMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

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


