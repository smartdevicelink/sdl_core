#include "JSONHandler/SetGlobalPropertiesMarshaller.h"
#include "ALRPCObjectsImpl/TTSChunkMarshaller.h"
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
  if (e.mHelpPrompt)
  {
    unsigned int sz=e.mHelpPrompt->size();
    j["helpPrompt"]=Json::Value(Json::arrayValue);
    j["helpPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["helpPrompt"][i]=TTSChunkMarshaller::toJSON(e.mHelpPrompt[0][i]);
  }
  if (e.mTimeoutPrompt)
  {
    unsigned int sz=e.mTimeoutPrompt->size();
    j["timeoutPrompt"]=Json::Value(Json::arrayValue);
    j["timeoutPrompt"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["timeoutPrompt"][i]=TTSChunkMarshaller::toJSON(e.mTimeoutPrompt[0][i]);  
  }  
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool SetGlobalPropertiesMarshaller::fromJSON(const Json::Value& json,SetGlobalProperties& c)
{      
  delete c.mHelpPrompt;
  delete c.mTimeoutPrompt;

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

    if(j.isMember("helpPrompt"))
    {
      const Json::Value& hp=j["helpPrompt"];
      if(!hp.isArray())  return false;
      c.mHelpPrompt=new std::vector<TTSChunk>(hp.size());
      for (int i = 0; i < hp.size(); i++ )
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(hp[i],t))
          return false;
        c.mHelpPrompt[0][i]=t;
      }      
    }    

    if(j.isMember("timeoutPrompt")) 
    {
      const Json::Value& tp=j["timeoutPrompt"];
      if(!tp.isArray())  return false;
      c.mTimeoutPrompt=new std::vector<TTSChunk>(tp.size());
      for(unsigned int i=0;i<tp.size();i++)
      {
        TTSChunk t;
        if(!TTSChunkMarshaller::fromJSON(tp[i],t))
          return false;
        c.mTimeoutPrompt[0][i]=t;
      }
      
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

