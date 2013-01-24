#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteraction.h"
#include "../src/ALRPCObjectsImpl/V2/TTSChunkMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/InteractionModeMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/TTSChunkMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/TTSChunkMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/VrHelpItemMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformInteractionMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool PerformInteractionMarshaller::checkIntegrity(PerformInteraction& s)
{
  return checkIntegrityConst(s);
}


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
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PerformInteractionMarshaller::checkIntegrityConst(const PerformInteraction& s)
{
  if(s.initialText.length()>500)  return false;

  {
    unsigned int i=s.initialPrompt.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  if(!NsAppLinkRPCV2::InteractionModeMarshaller::checkIntegrityConst(s.interactionMode))  return false;

  {
    unsigned int i=s.interactionChoiceSetIDList.size();
    if(i<1)  return false;
    if(i>100)  return false;
    while(i--)
    {
      if(s.interactionChoiceSetIDList[i]>2000000000)  return false;
    }
  }

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

  if(s.timeout && (s.timeout[0]<5000 || s.timeout[0]>100000))  return false;

  if(s.vrHelp)
  {
    unsigned int i=s.vrHelp[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value PerformInteractionMarshaller::toJSON(const PerformInteraction& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.PerformInteraction");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["initialText"]=Json::Value(e.initialText);;
  {
    unsigned int i=e.initialPrompt.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::TTSChunkMarshaller::toJSON(e.initialPrompt[i]);

    json["params"]["initialPrompt"]=j;
  }
  json["params"]["interactionMode"]=NsAppLinkRPCV2::InteractionModeMarshaller::toJSON(e.interactionMode);;
  {
    unsigned int i=e.interactionChoiceSetIDList.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.interactionChoiceSetIDList[i]);

    json["params"]["interactionChoiceSetIDList"]=j;
  }
  if(e.helpPrompt)
  {
    unsigned int i=e.helpPrompt[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::TTSChunkMarshaller::toJSON(e.helpPrompt[0][i]);

    json["params"]["helpPrompt"]=j;
  }
  if(e.timeoutPrompt)
  {
    unsigned int i=e.timeoutPrompt[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::TTSChunkMarshaller::toJSON(e.timeoutPrompt[0][i]);

    json["params"]["timeoutPrompt"]=j;
  }
  if(e.timeout)
    json["params"]["timeout"]=Json::Value(e.timeout[0]);;
  if(e.vrHelp)
  {
    unsigned int i=e.vrHelp[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPCV2::VrHelpItemMarshaller::toJSON(e.vrHelp[0][i]);

    json["params"]["vrHelp"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool PerformInteractionMarshaller::fromJSON(const Json::Value& json,PerformInteraction& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.PerformInteraction"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("initialText") || !js["initialText"].isString())  return false;
    c.initialText=js["initialText"].asString();
    if(c.initialText.length()>500)  return false;

    if(!js.isMember("initialPrompt") || !js["initialPrompt"].isArray())  return false;
    {
      unsigned int i=js["initialPrompt"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.initialPrompt.resize(i);
      while(i--)
      {
        NsAppLinkRPCV2::TTSChunk t;
        if(!NsAppLinkRPCV2::TTSChunkMarshaller::fromJSON(js["initialPrompt"][i],t))
          return false;
         c.initialPrompt[i]=t;
      }
    }

    if(!js.isMember("interactionMode") || !NsAppLinkRPCV2::InteractionModeMarshaller::fromJSON(js["interactionMode"],c.interactionMode))  return false;

    if(!js.isMember("interactionChoiceSetIDList") || !js["interactionChoiceSetIDList"].isArray())
      return false;
    {
      c.interactionChoiceSetIDList.clear();
      unsigned int i=js["interactionChoiceSetIDList"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.interactionChoiceSetIDList.resize(i);
      while(i--)
      {
        if(!js["interactionChoiceSetIDList"][i].isInt())
          return false;
        c.interactionChoiceSetIDList[i]=js["interactionChoiceSetIDList"][i].asInt();
        if(c.interactionChoiceSetIDList[i]>2000000000)  return false;

      }
    }


    if(c.helpPrompt)  delete c.helpPrompt;
    c.helpPrompt=0;
    if(js.isMember("helpPrompt"))
    {
      if(!js["helpPrompt"].isArray()) return false;
      unsigned int i=js["helpPrompt"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.helpPrompt=new std::vector<NsAppLinkRPCV2::TTSChunk>();
      c.helpPrompt->resize(js["helpPrompt"].size());

      while(i--)
        if(!NsAppLinkRPCV2::TTSChunkMarshaller::fromJSON(js["helpPrompt"][i],c.helpPrompt[0][i]))  return false;
    }


    if(c.timeoutPrompt)  delete c.timeoutPrompt;
    c.timeoutPrompt=0;
    if(js.isMember("timeoutPrompt"))
    {
      if(!js["timeoutPrompt"].isArray()) return false;
      unsigned int i=js["timeoutPrompt"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.timeoutPrompt=new std::vector<NsAppLinkRPCV2::TTSChunk>();
      c.timeoutPrompt->resize(js["timeoutPrompt"].size());

      while(i--)
        if(!NsAppLinkRPCV2::TTSChunkMarshaller::fromJSON(js["timeoutPrompt"][i],c.timeoutPrompt[0][i]))  return false;
    }


    if(c.timeout)  delete c.timeout;
    c.timeout=0;
    if(js.isMember("timeout"))
    {
      if(!js["timeout"].isInt())  return false;
      c.timeout=new unsigned int();
      c.timeout[0]=js["timeout"].asInt();
      if(c.timeout[0]<5000 || c.timeout[0]>100000)  return false;

    }

    if(c.vrHelp)  delete c.vrHelp;
    c.vrHelp=0;
    if(js.isMember("vrHelp"))
    {
      if(!js["vrHelp"].isArray()) return false;
      unsigned int i=js["vrHelp"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.vrHelp=new std::vector<NsAppLinkRPCV2::VrHelpItem>();
      c.vrHelp->resize(js["vrHelp"].size());

      while(i--)
        if(!NsAppLinkRPCV2::VrHelpItemMarshaller::fromJSON(js["vrHelp"][i],c.vrHelp[0][i]))  return false;
    }


    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
