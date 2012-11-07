#include "JSONHandler/DeleteInteractionChoiceSetMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool DeleteInteractionChoiceSetMarshaller::fromString(const std::string& s,DeleteInteractionChoiceSet& e)
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


const std::string DeleteInteractionChoiceSetMarshaller::toString(const DeleteInteractionChoiceSet& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value DeleteInteractionChoiceSetMarshaller::toJSON(const DeleteInteractionChoiceSet& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.DeleteInteractionChoiceSet");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["interactionChoiceSetID"] = Json::Value(e.interactionChoiceSetID);
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool DeleteInteractionChoiceSetMarshaller::fromJSON(const Json::Value& json,DeleteInteractionChoiceSet& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.DeleteInteractionChoiceSet"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("interactionChoiceSetID") || !j["interactionChoiceSetID"].isInt()) return false;
    c.interactionChoiceSetID = j["interactionChoiceSetID"].asInt();

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

