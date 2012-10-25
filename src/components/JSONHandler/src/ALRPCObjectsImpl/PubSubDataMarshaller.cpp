#include "../../include/JSONHandler/ALRPCObjects/PubSubData.h"
#include "PublishedDataMarshaller.h"

#include "PubSubDataMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/


bool PubSubDataMarshaller::checkIntegrity(PubSubData& s)
{
  return checkIntegrityConst(s);
}


bool PubSubDataMarshaller::fromString(const std::string& s,PubSubData& e)
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


const std::string PubSubDataMarshaller::toString(const PubSubData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PubSubDataMarshaller::checkIntegrityConst(const PubSubData& s)
{
  if(!PublishedDataMarshaller::checkIntegrityConst(s.data))  return false;
  return true;
}

Json::Value PubSubDataMarshaller::toJSON(const PubSubData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["data"]=PublishedDataMarshaller::toJSON(e.data);


  return json;
}


bool PubSubDataMarshaller::fromJSON(const Json::Value& json,PubSubData& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("data"))  return false;
    {
      const Json::Value& j=json["data"];
      if(!PublishedDataMarshaller::fromJSON(j,c.data))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

