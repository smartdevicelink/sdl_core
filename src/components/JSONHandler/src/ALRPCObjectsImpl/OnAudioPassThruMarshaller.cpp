#include "../include/JSONHandler/ALRPCObjects/OnAudioPassThru.h"


#include "OnAudioPassThruMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


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

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

