#include "../include/JSONHandler/ALRPCObjects/V2/ParameterPermissions.h"
#include "ParameterPermissionsMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPCV2;

ParameterPermissions::ParameterPermissions(const ParameterPermissions& c)
{
  *this=c;
}


bool ParameterPermissions::checkIntegrity(void)
{
  return ParameterPermissionsMarshaller::checkIntegrity(*this);
}


ParameterPermissions::ParameterPermissions(void)
{
}



bool ParameterPermissions::set_allowed(const std::vector<std::string>& allowed_)
{
  unsigned int i=allowed_.size();
  if(i>100 || i<0)  return false;
  while(i--)
  {
  }
  allowed=allowed_;
  return true;
}

bool ParameterPermissions::set_userDisallowed(const std::vector<std::string>& userDisallowed_)
{
  unsigned int i=userDisallowed_.size();
  if(i>100 || i<0)  return false;
  while(i--)
  {
  }
  userDisallowed=userDisallowed_;
  return true;
}




const std::vector<std::string>& ParameterPermissions::get_allowed(void) const 
{
  return allowed;
}


const std::vector<std::string>& ParameterPermissions::get_userDisallowed(void) const 
{
  return userDisallowed;
}


