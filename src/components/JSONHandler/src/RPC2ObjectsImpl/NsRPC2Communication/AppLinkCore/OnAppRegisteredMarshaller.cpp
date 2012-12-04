#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "../src/ALRPCObjectsImpl/V1/LanguageMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/LanguageMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/TTSChunkMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/AppTypeMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppRegisteredMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

bool OnAppRegisteredMarshaller::checkIntegrity(OnAppRegistered& s)
{
  return checkIntegrityConst(s);
}


bool OnAppRegisteredMarshaller::fromString(const std::string& s,OnAppRegistered& e)
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


const std::string OnAppRegisteredMarshaller::toString(const OnAppRegistered& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppRegisteredMarshaller::checkIntegrityConst(const OnAppRegistered& s)
{
  if(s.appName.length()>100)  return false;

  if(s.vrSynonym)
  {
    unsigned int i=s.vrSynonym[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
    while(i--)
    {
      if(s.vrSynonym[0][i].length()>40)  return false;
    }
  }

  if(!NsAppLinkRPC::LanguageMarshaller::checkIntegrityConst(s.languageDesired))  return false;

  if(!NsAppLinkRPC::LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguageDesired))  return false;

  if(s.ttsName)
  {
    unsigned int i=s.ttsName[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  if(s.appType)
  {
    unsigned int i=s.appType[0].size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value OnAppRegisteredMarshaller::toJSON(const OnAppRegistered& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.OnAppRegistered");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["appName"]=Json::Value(e.appName);;
  json["params"]["appIcon"]=Json::Value(e.appIcon);;
  json["params"]["deviceName"]=Json::Value(e.deviceName);;
  if(e.vrSynonym)
  {
    unsigned int i=e.vrSynonym[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.vrSynonym[0][i]);

    json["params"]["vrSynonym"]=j;
  }
  json["params"]["isMediaApplication"]=Json::Value(e.isMediaApplication);;
  json["params"]["languageDesired"]=NsAppLinkRPC::LanguageMarshaller::toJSON(e.languageDesired);;
  json["params"]["hmiDisplayLanguageDesired"]=NsAppLinkRPC::LanguageMarshaller::toJSON(e.hmiDisplayLanguageDesired);;
  if(e.ttsName)
  {
    unsigned int i=e.ttsName[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPC::TTSChunkMarshaller::toJSON(e.ttsName[0][i]);

    json["params"]["ttsName"]=j;
  }
  if(e.appType)
  {
    unsigned int i=e.appType[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPC::AppTypeMarshaller::toJSON(e.appType[0][i]);

    json["params"]["appType"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool OnAppRegisteredMarshaller::fromJSON(const Json::Value& json,OnAppRegistered& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.OnAppRegistered"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("appName") || !js["appName"].isString())  return false;
    c.appName=js["appName"].asString();
    if(c.appName.length()>100)  return false;

    if(!js.isMember("appIcon") || !js["appIcon"].isString())  return false;
    c.appIcon=js["appIcon"].asString();
    
    if(!js.isMember("deviceName") || !js["deviceName"].isString())  return false;
    c.deviceName=js["deviceName"].asString();
    
    if(c.vrSynonym)  delete c.vrSynonym;
    c.vrSynonym=0;
    if(js.isMember("vrSynonym"))
    {
      if(!js["vrSynonym"].isArray()) return false;
      unsigned int i=js["vrSynonym"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.vrSynonym=new std::vector<std::string>();
      c.vrSynonym->resize(js["vrSynonym"].size());

      while(i--)
      {
        if(!js["vrSynonym"][i].isString())
          return false;

        c.vrSynonym[0][i]=js["vrSynonym"][i].asString();
        if(c.vrSynonym[0][i].length()>40)  return false;
      }
    }


    if(!js.isMember("isMediaApplication") || !js["isMediaApplication"].isBool())  return false;
    c.isMediaApplication=js["isMediaApplication"].asBool();
    
    if(!js.isMember("languageDesired") || !NsAppLinkRPC::LanguageMarshaller::fromJSON(js["languageDesired"],c.languageDesired))  return false;

    if(!js.isMember("hmiDisplayLanguageDesired") || !NsAppLinkRPC::LanguageMarshaller::fromJSON(js["hmiDisplayLanguageDesired"],c.hmiDisplayLanguageDesired))  return false;

    if(c.ttsName)  delete c.ttsName;
    c.ttsName=0;
    if(js.isMember("ttsName"))
    {
      if(!js["ttsName"].isArray()) return false;
      unsigned int i=js["ttsName"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.ttsName=new std::vector<NsAppLinkRPC::TTSChunk>();
      c.ttsName->resize(js["ttsName"].size());

      while(i--)
        if(!NsAppLinkRPC::TTSChunkMarshaller::fromJSON(js["ttsName"][i],c.ttsName[0][i]))  return false;
    }


    if(c.appType)  delete c.appType;
    c.appType=0;
    if(js.isMember("appType"))
    {
      if(!js["appType"].isArray()) return false;
      unsigned int i=js["appType"].size();
      if(i<1)  return false;
      if(i>100)  return false;

      c.appType=new std::vector<NsAppLinkRPC::AppType>();
      c.appType->resize(js["appType"].size());

      while(i--)
        if(!NsAppLinkRPC::AppTypeMarshaller::fromJSON(js["appType"][i],c.appType[0][i]))  return false;
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
