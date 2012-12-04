#include "../include/JSONHandler/ALRPCObjects/V2/OnDriverDistraction.h"
#include "DriverDistractionStateMarshaller.h"

#include "OnDriverDistractionMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


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

  json["state"]=DriverDistractionStateMarshaller::toJSON(e.state);

  return json;
}


bool OnDriverDistractionMarshaller::fromJSON(const Json::Value& json,OnDriverDistraction& c)
{
  try
  {
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

