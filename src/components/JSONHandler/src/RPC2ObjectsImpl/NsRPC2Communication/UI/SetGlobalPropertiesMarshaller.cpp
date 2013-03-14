#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetGlobalProperties.h"
#include "../src/SDLRPCObjectsImpl/V2/TTSChunkMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/TTSChunkMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/VrHelpItemMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SetGlobalPropertiesMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

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

  if(s.vrHelpTitle && (s.vrHelpTitle[0].length()>500))  return false;

  if(s.vrHelp)
  {
    unsigned int i=s.vrHelp[0].size();
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
      j[i]=NsSmartDeviceLinkRPCV2::TTSChunkMarshaller::toJSON(e.helpPrompt[0][i]);

    json["params"]["helpPrompt"]=j;
  }
  if(e.timeoutPrompt)
  {
    unsigned int i=e.timeoutPrompt[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::TTSChunkMarshaller::toJSON(e.timeoutPrompt[0][i]);

    json["params"]["timeoutPrompt"]=j;
  }
  if(e.vrHelpTitle)
    json["params"]["vrHelpTitle"]=Json::Value(e.vrHelpTitle[0]);;
  if(e.vrHelp)
  {
    unsigned int i=e.vrHelp[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsSmartDeviceLinkRPCV2::VrHelpItemMarshaller::toJSON(e.vrHelp[0][i]);

    json["params"]["vrHelp"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
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

      c.helpPrompt=new std::vector<NsSmartDeviceLinkRPCV2::TTSChunk>();
      c.helpPrompt->resize(js["helpPrompt"].size());

      while(i--)
        if(!NsSmartDeviceLinkRPCV2::TTSChunkMarshaller::fromJSON(js["helpPrompt"][i],c.helpPrompt[0][i]))  return false;
    }


    if(c.timeoutPrompt)  delete c.timeoutPrompt;
    c.timeoutPrompt=0;
    if(js.isMember("timeoutPrompt"))
    {
      if(!js["timeoutPrompt"].isArray()) return false;
      unsigned int i=js["timeoutPrompt"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.timeoutPrompt=new std::vector<NsSmartDeviceLinkRPCV2::TTSChunk>();
      c.timeoutPrompt->resize(js["timeoutPrompt"].size());

      while(i--)
        if(!NsSmartDeviceLinkRPCV2::TTSChunkMarshaller::fromJSON(js["timeoutPrompt"][i],c.timeoutPrompt[0][i]))  return false;
    }


    if(c.vrHelpTitle)  delete c.vrHelpTitle;
    c.vrHelpTitle=0;
    if(js.isMember("vrHelpTitle"))
    {
      if(!js["vrHelpTitle"].isString())  return false;
      c.vrHelpTitle=new std::string();
      c.vrHelpTitle[0]=js["vrHelpTitle"].asString();
      if(c.vrHelpTitle[0].length()>500)  return false;

    }

    if(c.vrHelp)  delete c.vrHelp;
    c.vrHelp=0;
    if(js.isMember("vrHelp"))
    {
      if(!js["vrHelp"].isArray()) return false;
      unsigned int i=js["vrHelp"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.vrHelp=new std::vector<NsSmartDeviceLinkRPCV2::VrHelpItem>();
      c.vrHelp->resize(js["vrHelp"].size());

      while(i--)
        if(!NsSmartDeviceLinkRPCV2::VrHelpItemMarshaller::fromJSON(js["vrHelp"][i],c.vrHelp[0][i]))  return false;
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
