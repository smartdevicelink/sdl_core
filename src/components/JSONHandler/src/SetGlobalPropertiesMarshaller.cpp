#include "JSONHandler/SetGlobalPropertiesMarshaller.h"
#include "JSONHandler/TTSChunkMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool SetGlobalPropertiesMarshaller::fromString(const std::string& s,SetGlobalProperties& e)
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


const std::string SetGlobalPropertiesMarshaller::toString(const SetGlobalProperties& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value SetGlobalPropertiesMarshaller::toJSON(const SetGlobalProperties& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.SetGlobalProperties");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["helpPrompt"]=TTSChunkMarshaller::toJSON(e.mHelpPrompt);
  j["timeoutPrompt"] = TTSChunkMarshaller::toJSON(e.mTimeoutPrompt);
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool SetGlobalPropertiesMarshaller::fromJSON(const Json::Value& json,SetGlobalProperties& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.SetGlobalProperties"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("helpPrompt"))  return false;
    if(!TTSChunkMarshaller::fromJSON(j["helpPrompt"], c.mHelpPrompt)) return false;

    if(!j.isMember("timeoutPrompt")) return false;
    if(!TTSChunkMarshaller::fromJSON(j["timeoutPrompt"], c.mTimeoutPrompt)) return false;

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

