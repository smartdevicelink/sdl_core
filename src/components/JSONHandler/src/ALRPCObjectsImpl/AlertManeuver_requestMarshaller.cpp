#include "../include/JSONHandler/ALRPCObjects/AlertManeuver_request.h"
#include "SoftButtonMarshaller.h"
#include "TTSChunk_v2Marshaller.h"

#include "AlertManeuver_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool AlertManeuver_requestMarshaller::checkIntegrity(AlertManeuver_request& s)
{
  return checkIntegrityConst(s);
}


bool AlertManeuver_requestMarshaller::fromString(const std::string& s,AlertManeuver_request& e)
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


const std::string AlertManeuver_requestMarshaller::toString(const AlertManeuver_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AlertManeuver_requestMarshaller::checkIntegrityConst(const AlertManeuver_request& s)
{
  {
    unsigned int i=s.ttsChunks.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!TTSChunk_v2Marshaller::checkIntegrityConst(s.ttsChunks[i]))   return false;
    }
  }
  {
    unsigned int i=s.softButtons.size();
    if(i>3 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[i]))   return false;
    }
  }
  return true;
}

Json::Value AlertManeuver_requestMarshaller::toJSON(const AlertManeuver_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("AlertManeuver");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  j["ttsChunks"]=Json::Value(Json::arrayValue);
  j["ttsChunks"].resize(e.ttsChunks.size());
  for(unsigned int i=0;i<e.ttsChunks.size();i++)
    j["ttsChunks"][i]=TTSChunk_v2Marshaller::toJSON(e.ttsChunks[i]);

  j["softButtons"]=Json::Value(Json::arrayValue);
  j["softButtons"].resize(e.softButtons.size());
  for(unsigned int i=0;i<e.softButtons.size();i++)
    j["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[i]);

  json["request"]["parameters"]=j;
  return json;
}


bool AlertManeuver_requestMarshaller::fromJSON(const Json::Value& js,AlertManeuver_request& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("AlertManeuver"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("ttsChunks"))  return false;
    {
      const Json::Value& j=json["ttsChunks"];
      if(!j.isArray())  return false;
      c.ttsChunks.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          TTSChunk_v2 t;
          if(!TTSChunk_v2Marshaller::fromJSON(j[i],t))
            return false;
          c.ttsChunks[i]=t;
        }

    }
    if(!json.isMember("softButtons"))  return false;
    {
      const Json::Value& j=json["softButtons"];
      if(!j.isArray())  return false;
      c.softButtons.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          SoftButton t;
          if(!SoftButtonMarshaller::fromJSON(j[i],t))
            return false;
          c.softButtons[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

