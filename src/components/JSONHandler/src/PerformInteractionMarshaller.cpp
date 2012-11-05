#include "JSONHandler/PerformInteractionMarshaller.h"
#include "ALRPCObjectsImpl/InteractionModeMarshaller.h"
#include "ALRPCObjectsImpl/TTSChunkMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool PerformInteractionMarshaller::fromString(const std::string& s,PerformInteraction& e)
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


const std::string PerformInteractionMarshaller::toString(const PerformInteraction& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value PerformInteractionMarshaller::toJSON(const PerformInteraction& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.PerformInteraction");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["initialText"] = Json::Value(e.initialText);
  j["initialPrompt"] = Json::Value(Json::arrayValue);
  j["initialPrompt"].resize(e.initialPrompt.size());
  for ( int i = 0; i < e.initialPrompt.size(); ++i )
  {
    j["initialPrompt"][i] = TTSChunkMarshaller::toJSON(e.initialPrompt[i]);
  }

  j["interactionMode"] = InteractionModeMarshaller::toJSON(e.interactionMode);

  j["interactionChoiceSetIDList"] = Json::Value(Json::arrayValue);
  j["interactionChoiceSetIDList"].resize(e.interactionChoiceSetIDList.size());
  for (int i = 0; i < e.interactionChoiceSetIDList.size(); ++i)
  {
    j["interactionChoiceSetIDList"][i] = Json::Value(e.interactionChoiceSetIDList[i]);
  }

  if (e.helpPrompt)
  {
    j["helpPrompt"] = Json::Value(Json::arrayValue);
    j["helpPrompt"].resize(e.helpPrompt->size());
    for (int i = 0; i < e.helpPrompt->size(); ++i)
    {
        j["helpPrompt"][i] = TTSChunkMarshaller::toJSON(e.helpPrompt[0][i]);
    }
  }

  if (e.timeoutPrompt)
  {
    j["timeoutPrompt"] = Json::Value(Json::arrayValue);
    j["timeoutPrompt"].resize(e.timeoutPrompt->size());
    for (int i = 0; i < e.timeoutPrompt->size(); ++i)
    {
      j["timeoutPrompt"][i] = TTSChunkMarshaller::toJSON(e.timeoutPrompt[0][i]);
    }
  }

  if (e.timeout)
  {
    j["timeout"] = Json::Value(*e.timeout);
  }

  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool PerformInteractionMarshaller::fromJSON(const Json::Value& json,PerformInteraction& c)
{
  if (c.helpPrompt)
        delete c.helpPrompt;
  if (c.timeoutPrompt)
    delete c.timeoutPrompt;
  if (c.timeout)
    delete c.timeout;

  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if(json["method"].asString().compare("UI.PerformInteraction"))
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if (!j.isMember("initialText") || !j["initialText"].isString()) return false;
    c.initialText = j["initialText"].asString();

    if (!j.isMember("initialPrompt") || !j["initialPrompt"].isArray()) return false;
    c.initialPrompt.clear();
    const Json::Value &v = j["initialPrompt"];
    for (int i = 0; i < v.size(); ++i)
    {
      TTSChunk chunk;
      if (!TTSChunkMarshaller::fromJSON(v[i],chunk)) return false;
      c.initialPrompt.push_back(chunk);
    }

    if (!j.isMember("interactionMode")) return false;
    if (!InteractionModeMarshaller::fromJSON(j["interactionMode"],c.interactionMode)) return false;

    if (!j.isMember("interactionChoiceSetIDList") || !j["interactionChoiceSetIDList"].isArray()) return false;
    c.interactionChoiceSetIDList.clear();
    const Json::Value& chset = j["interactionChoiceSetIDList"];
    for (int i = 0; i < chset.size(); ++i)
    {
      if(!chset[i].isInt()) return false;
      c.interactionChoiceSetIDList.push_back(chset[i].asInt());
    }

    if (j.isMember("helpPrompt"))
    {      
      const Json::Value & hp = j["helpPrompt"];
      c.helpPrompt = new std::vector<TTSChunk> (j["helpPrompt"].size());      
      for (int i = 0; i < hp.size(); ++i)
      {
        if (!TTSChunkMarshaller::fromJSON(hp[i],c.helpPrompt[0][i])) return false;
      }
    }

    if (j.isMember("timeoutPrompt"))
    {
      const Json::Value & tp = j["timeoutPrompt"];
      c.timeoutPrompt = new std::vector<TTSChunk>(tp.size());
      for (int i = 0; i < tp.size(); ++i)
      {
        if (!TTSChunkMarshaller::fromJSON(tp[i],c.timeoutPrompt[0][i])) return false;
      }
    }

    if (j.isMember("timeout"))
    {      
      if (!j["timeout"].isInt()) return false;
      c.timeout = new unsigned int(j["timeout"].asInt());
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

