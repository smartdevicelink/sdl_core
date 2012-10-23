#include "JSONHandler/TTSChunkMarshaller.h"
#include "JSONHandler/SpeechCapabilitiesMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

bool TTSChunkMarshaller::fromJSON(const Json::Value& s,TTSChunk& e)
{
  if(!s["text"].isString())
    return false;

  e.mText = s["text"].asString();

  if (!s.isMember("type"))
    return false;
  Json::Value j = s["type"];
  if (!SpeechCapabilitiesMarshaller::fromJSON(j,e.mType))
    return false;

  return true;
}


Json::Value TTSChunkMarshaller::toJSON(const TTSChunk& e)
{
  Json::Value json(Json::objectValue);
  json["text"] = Json::Value(e.mText);
  json["type"] = SpeechCapabilitiesMarshaller::toJSON(e.mType);
  
  return json;
}


bool TTSChunkMarshaller::fromString(const std::string& s,TTSChunk& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string TTSChunkMarshaller::toString(const TTSChunk& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}

