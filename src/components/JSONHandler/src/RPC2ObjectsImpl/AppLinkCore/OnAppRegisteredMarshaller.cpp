#include "../include/JSONHandler/RPC2Objects/AppLinkCore/OnAppRegistered.h"
#include "../src/ALRPCObjectsImpl/LanguageMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "OnAppRegisteredMarshaller.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:25:43 2012
  source stamp	Wed Nov  7 09:35:35 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::AppLinkCore;

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

  if(!LanguageMarshaller::checkIntegrityConst(s.languageDesired))  return false;

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
  json["params"]["languageDesired"]=LanguageMarshaller::toJSON(e.languageDesired);;
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
    
    if(!js.isMember("languageDesired") || !LanguageMarshaller::fromJSON(js["languageDesired"],c.languageDesired))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
