#include "../../include/JSONHandler/ALRPCObjects/OnAudioPassThru.h"
#include "DataTypeMarshaller.h"

#include "OnAudioPassThruMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool OnAudioPassThruMarshaller::checkIntegrity(OnAudioPassThru& s)
{
  return checkIntegrityConst(s);
}


bool OnAudioPassThruMarshaller::fromString(const std::string& s,OnAudioPassThru& e)
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


const std::string OnAudioPassThruMarshaller::toString(const OnAudioPassThru& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAudioPassThruMarshaller::checkIntegrityConst(const OnAudioPassThru& s)
{
  if(!DataTypeMarshaller::checkIntegrityConst(s.audioPacket))  return false;
  return true;
}

Json::Value OnAudioPassThruMarshaller::toJSON(const OnAudioPassThru& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnAudioPassThru");

  Json::Value j=Json::Value(Json::objectValue);

  j["audioPacket"]=DataTypeMarshaller::toJSON(e.audioPacket);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnAudioPassThruMarshaller::fromJSON(const Json::Value& js,OnAudioPassThru& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnAudioPassThru"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("audioPacket"))  return false;
    {
      const Json::Value& j=json["audioPacket"];
      if(!DataTypeMarshaller::fromJSON(j,c.audioPacket))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

