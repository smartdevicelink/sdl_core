#include "JSONHandler/CreateInteractionChoiceSetMarshaller.h"
#include "ALRPCObjectsImpl/ChoiceMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool CreateInteractionChoiceSetMarshaller::fromString(const std::string& s,CreateInteractionChoiceSet& e)
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


const std::string CreateInteractionChoiceSetMarshaller::toString(const CreateInteractionChoiceSet& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value CreateInteractionChoiceSetMarshaller::toJSON(const CreateInteractionChoiceSet& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.CreateInteractionChoiceSet");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["interactionChoiceSetID"] = Json::Value(e.interactionChoiceSetID);
  j["choiceSet"] = Json::Value(Json::arrayValue);
  j["choiceSet"].resize(e.choiceSet.size());
  for ( int i = 0; i < e.choiceSet.size(); ++i )
  {
    j["choiceSet"][i] = ChoiceMarshaller::toJSON(e.choiceSet[i]);
  }
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool CreateInteractionChoiceSetMarshaller::fromJSON(const Json::Value& json,CreateInteractionChoiceSet& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.CreateInteractionChoiceSet"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("interactionChoiceSetID") || !j["interactionChoiceSetID"].isInt()) return false;
    c.interactionChoiceSetID = j["interactionChoiceSetID"].asInt();

    if(!j.isMember("choiceSet") || !j["choiceSet"].isArray()) return false;
    const Json::Value js = j["choiceSet"];
    c.choiceSet.clear();
    c.choiceSet.resize(js.size());
    for ( int i = 0; i < js.size(); ++i )
    {
      Choice choice;
      if (!ChoiceMarshaller::fromJSON(js[i], choice)) return false;
      c.choiceSet.push_back(choice);
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

