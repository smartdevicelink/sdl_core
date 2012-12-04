#include "../include/JSONHandler/ALRPCObjects/V2/OnAppInterfaceUnregistered.h"
#include "AppInterfaceUnregisteredReasonMarshaller.h"

#include "OnAppInterfaceUnregisteredMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool OnAppInterfaceUnregisteredMarshaller::checkIntegrity(OnAppInterfaceUnregistered& s)
{
  return checkIntegrityConst(s);
}


bool OnAppInterfaceUnregisteredMarshaller::fromString(const std::string& s,OnAppInterfaceUnregistered& e)
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


const std::string OnAppInterfaceUnregisteredMarshaller::toString(const OnAppInterfaceUnregistered& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppInterfaceUnregisteredMarshaller::checkIntegrityConst(const OnAppInterfaceUnregistered& s)
{
  if(!AppInterfaceUnregisteredReasonMarshaller::checkIntegrityConst(s.reason))  return false;
  return true;
}

Json::Value OnAppInterfaceUnregisteredMarshaller::toJSON(const OnAppInterfaceUnregistered& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["reason"]=AppInterfaceUnregisteredReasonMarshaller::toJSON(e.reason);

  return json;
}


bool OnAppInterfaceUnregisteredMarshaller::fromJSON(const Json::Value& json,OnAppInterfaceUnregistered& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("reason"))  return false;
    {
      const Json::Value& j=json["reason"];
      if(!AppInterfaceUnregisteredReasonMarshaller::fromJSON(j,c.reason))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

