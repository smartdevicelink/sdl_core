#include "../include/JSONHandler/ALRPCObjects/V2/SingleTireStatus.h"
#include "SingleTirePressureStatusMarshaller.h"

#include "SingleTireStatusMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool SingleTireStatusMarshaller::checkIntegrity(SingleTireStatus& s)
{
  return checkIntegrityConst(s);
}


bool SingleTireStatusMarshaller::fromString(const std::string& s,SingleTireStatus& e)
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


const std::string SingleTireStatusMarshaller::toString(const SingleTireStatus& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SingleTireStatusMarshaller::checkIntegrityConst(const SingleTireStatus& s)
{
  if(s.pressure>255)  return false;
  if(!SingleTirePressureStatusMarshaller::checkIntegrityConst(s.status))  return false;
  return true;
}

Json::Value SingleTireStatusMarshaller::toJSON(const SingleTireStatus& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["pressure"]=Json::Value(e.pressure);

  json["status"]=SingleTirePressureStatusMarshaller::toJSON(e.status);


  return json;
}


bool SingleTireStatusMarshaller::fromJSON(const Json::Value& json,SingleTireStatus& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("pressure"))  return false;
    {
      const Json::Value& j=json["pressure"];
      if(!j.isInt())  return false;
      c.pressure=j.asInt();
    }
    if(!json.isMember("status"))  return false;
    {
      const Json::Value& j=json["status"];
      if(!SingleTirePressureStatusMarshaller::fromJSON(j,c.status))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

