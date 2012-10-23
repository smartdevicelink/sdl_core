#include "JSONHandler/SpeakMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include "JSONHandler/TTSChunkMarshaller.h"

using namespace RPC2Communication;

bool SpeakMarshaller::fromString(const std::string& s,Speak& e)
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


const std::string SpeakMarshaller::toString(const Speak& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value SpeakMarshaller::toJSON(const Speak& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("TTS.Speak");
  
  Json::Value j=Json::Value(Json::arrayValue);
  for ( int i = 0; i < e.mTTSChunks.size(); ++i )
  {
    j.append(TTSChunkMarshaller::toJSON(e.mTTSChunks[i]));
  }
    
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["ttsChunks"] = j;

  json["id"] = e.getID();

  return json;
}


bool SpeakMarshaller::fromJSON(const Json::Value& json,Speak& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("TTS.Speak"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("ttsChunks") || !j["ttsChunks"].isArray())  return false;
    j = j["ttsChunks"];
    for ( int i = 0; i < j.size(); i++ )
    {
      TTSChunk chunk;
      if(!TTSChunkMarshaller::fromJSON(j[i],chunk)) return false;  
      c.mTTSChunks.push_back(chunk);
    }    

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}
