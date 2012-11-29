#include "../include/JSONHandler/ALRPCObjects/ParameterPermissions.h"
#include "ParameterPermissionsMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

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


