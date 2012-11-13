#include "../include/JSONHandler/RPC2Objects/UI/SetGlobalProperties.h"
#include "../src/ALRPCObjectsImpl/TTSChunkMarshaller.h"
#include "../src/ALRPCObjectsImpl/TTSChunkMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "SetGlobalPropertiesMarshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;

bool SetGlobalPropertiesMarshaller::checkIntegrity(SetGlobalProperties& s)
{
  return checkIntegrityConst(s);
}


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
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetGlobalPropertiesMarshaller::checkIntegrityConst(const SetGlobalProperties& s)
{
  if(s.helpPrompt)
  {
    unsigned int i=s.helpPrompt[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  if(s.timeoutPrompt)
  {
    unsigned int i=s.timeoutPrompt[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value SetGlobalPropertiesMarshaller::toJSON(const SetGlobalProperties& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.SetGlobalProperties");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  if(e.helpPrompt)
  {
    unsigned int i=e.helpPrompt[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=TTSChunkMarshaller::toJSON(e.helpPrompt[0][i]);

    json["params"]["helpPrompt"]=j;
  }
  if(e.timeoutPrompt)
  {
    unsigned int i=e.timeoutPrompt[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=TTSChunkMarshaller::toJSON(e.timeoutPrompt[0][i]);

    json["params"]["timeoutPrompt"]=j;
  }
  return json;
}


bool SetGlobalPropertiesMarshaller::fromJSON(const Json::Value& json,SetGlobalProperties& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.SetGlobalProperties"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(c.helpPrompt)  delete c.helpPrompt;
    c.helpPrompt=0;
    if(js.isMember("helpPrompt"))
    {
      if(!js["helpPrompt"].isArray()) return false;
      unsigned int i=js["helpPrompt"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.helpPrompt=new std::vector<TTSChunk>();
      c.helpPrompt->resize(js["helpPrompt"].size());

      while(i--)
        if(!TTSChunkMarshaller::fromJSON(js["helpPrompt"][i],c.helpPrompt[0][i]))  return false;
    }


    if(c.timeoutPrompt)  delete c.timeoutPrompt;
    c.timeoutPrompt=0;
    if(js.isMember("timeoutPrompt"))
    {
      if(!js["timeoutPrompt"].isArray()) return false;
      unsigned int i=js["timeoutPrompt"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.timeoutPrompt=new std::vector<TTSChunk>();
      c.timeoutPrompt->resize(js["timeoutPrompt"].size());

      while(i--)
        if(!TTSChunkMarshaller::fromJSON(js["timeoutPrompt"][i],c.timeoutPrompt[0][i]))  return false;
    }


  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
