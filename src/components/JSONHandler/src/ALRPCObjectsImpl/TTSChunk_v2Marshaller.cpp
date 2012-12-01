#include "../include/JSONHandler/ALRPCObjects/TTSChunk_v2.h"
#include "SpeechCapabilities_v2Marshaller.h"

#include "TTSChunk_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool TTSChunk_v2Marshaller::checkIntegrity(TTSChunk_v2& s)
{
  return checkIntegrityConst(s);
}


bool TTSChunk_v2Marshaller::fromString(const std::string& s,TTSChunk_v2& e)
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


const std::string TTSChunk_v2Marshaller::toString(const TTSChunk_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool TTSChunk_v2Marshaller::checkIntegrityConst(const TTSChunk_v2& s)
{
  if(s.text.length()>500)  return false;
  if(!SpeechCapabilities_v2Marshaller::checkIntegrityConst(s.type))  return false;
  return true;
}

Json::Value TTSChunk_v2Marshaller::toJSON(const TTSChunk_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["text"]=Json::Value(e.text);

  json["type"]=SpeechCapabilities_v2Marshaller::toJSON(e.type);


  return json;
}


bool TTSChunk_v2Marshaller::fromJSON(const Json::Value& json,TTSChunk_v2& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("text"))  return false;
    {
      const Json::Value& j=json["text"];
      if(!j.isString())  return false;
      c.text=j.asString();
    }
    if(!json.isMember("type"))  return false;
    {
      const Json::Value& j=json["type"];
      if(!SpeechCapabilities_v2Marshaller::fromJSON(j,c.type))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

