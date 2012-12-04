#include "../include/JSONHandler/ALRPCObjects/V2/HMIPermissions.h"
#include "HMILevelMarshaller.h"

#include "HMIPermissionsMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool HMIPermissionsMarshaller::checkIntegrity(HMIPermissions& s)
{
  return checkIntegrityConst(s);
}


bool HMIPermissionsMarshaller::fromString(const std::string& s,HMIPermissions& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string HMIPermissionsMarshaller::toString(const HMIPermissions& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool HMIPermissionsMarshaller::checkIntegrityConst(const HMIPermissions& s)
{
  {
    unsigned int i=s.allowed.size();
    if(i>100 || i<0)  return false;
    while(i--)
    {
    if(!HMILevelMarshaller::checkIntegrityConst(s.allowed[i]))   return false;
    }
  }
  {
    unsigned int i=s.userDisallowed.size();
    if(i>100 || i<0)  return false;
    while(i--)
    {
    if(!HMILevelMarshaller::checkIntegrityConst(s.userDisallowed[i]))   return false;
    }
  }
  return true;
}

Json::Value HMIPermissionsMarshaller::toJSON(const HMIPermissions& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["allowed"]=Json::Value(Json::arrayValue);
  json["allowed"].resize(e.allowed.size());
  for(unsigned int i=0;i<e.allowed.size();i++)
    json["allowed"][i]=HMILevelMarshaller::toJSON(e.allowed[i]);

  json["userDisallowed"]=Json::Value(Json::arrayValue);
  json["userDisallowed"].resize(e.userDisallowed.size());
  for(unsigned int i=0;i<e.userDisallowed.size();i++)
    json["userDisallowed"][i]=HMILevelMarshaller::toJSON(e.userDisallowed[i]);


  return json;
}


bool HMIPermissionsMarshaller::fromJSON(const Json::Value& json,HMIPermissions& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("allowed"))  return false;
    {
      const Json::Value& j=json["allowed"];
      if(!j.isArray())  return false;
      c.allowed.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          HMILevel t;
          if(!HMILevelMarshaller::fromJSON(j[i],t))
            return false;
          c.allowed[i]=t;
        }

    }
    if(!json.isMember("userDisallowed"))  return false;
    {
      const Json::Value& j=json["userDisallowed"];
      if(!j.isArray())  return false;
      c.userDisallowed.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          HMILevel t;
          if(!HMILevelMarshaller::fromJSON(j[i],t))
            return false;
          c.userDisallowed[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

