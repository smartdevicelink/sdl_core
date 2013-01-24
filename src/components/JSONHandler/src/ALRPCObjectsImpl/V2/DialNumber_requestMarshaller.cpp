#include "../include/JSONHandler/ALRPCObjects/V2/DialNumber_request.h"


#include "DialNumber_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool DialNumber_requestMarshaller::checkIntegrity(DialNumber_request& s)
{
  return checkIntegrityConst(s);
}


bool DialNumber_requestMarshaller::fromString(const std::string& s,DialNumber_request& e)
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


const std::string DialNumber_requestMarshaller::toString(const DialNumber_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool DialNumber_requestMarshaller::checkIntegrityConst(const DialNumber_request& s)
{
  if(s.number.length()>500)  return false;
  return true;
}

Json::Value DialNumber_requestMarshaller::toJSON(const DialNumber_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["number"]=Json::Value(e.number);

  return json;
}


bool DialNumber_requestMarshaller::fromJSON(const Json::Value& json,DialNumber_request& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("number"))  return false;
    {
      const Json::Value& j=json["number"];
      if(!j.isString())  return false;
      c.number=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

