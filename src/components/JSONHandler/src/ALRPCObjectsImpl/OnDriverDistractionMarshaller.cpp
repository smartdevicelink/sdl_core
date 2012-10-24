#include "../../include/JSONHandler/ALRPCObjects/OnDriverDistraction.h"
#include "DriverDistractionStateMarshaller.h"

#include "OnDriverDistractionMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool OnDriverDistractionMarshaller::checkIntegrity(OnDriverDistraction& s)
{
  return checkIntegrityConst(s);
}


bool OnDriverDistractionMarshaller::fromString(const std::string& s,OnDriverDistraction& e)
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


const std::string OnDriverDistractionMarshaller::toString(const OnDriverDistraction& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnDriverDistractionMarshaller::checkIntegrityConst(const OnDriverDistraction& s)
{
  if(!DriverDistractionStateMarshaller::checkIntegrityConst(s.state))  return false;
  return true;
}

Json::Value OnDriverDistractionMarshaller::toJSON(const OnDriverDistraction& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnDriverDistraction");

  Json::Value j=Json::Value(Json::objectValue);

  j["state"]=DriverDistractionStateMarshaller::toJSON(e.state);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnDriverDistractionMarshaller::fromJSON(const Json::Value& js,OnDriverDistraction& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnDriverDistraction"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("state"))  return false;
    {
      const Json::Value& j=json["state"];
      if(!DriverDistractionStateMarshaller::fromJSON(j,c.state))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

